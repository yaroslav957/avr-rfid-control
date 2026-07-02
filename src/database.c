#include "database.h"
#include "error.h"

#include <assert.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <string.h>

static_assert(INIT_USERS <= MAX_USERS,
              "Flash database can't be bigger than EEPROM");

static const User PROGMEM init_database[INIT_USERS] = {
    {.hash = {0x84, 0xE5, 0xA2, 0x9B, 0x07, 0x1D, 0x8C, 0x67, 0x5D, 0x2A, 0x11,
              0xEB, 0xEE, 0x22, 0x53, 0x13, 0x6A, 0x74, 0x26, 0x5A, 0xE4, 0x00,
              0x5A, 0x3D, 0xD6, 0x4F, 0xAE, 0x0E, 0x4A, 0xBC, 0xEF, 0x98},
     .name = "Admin"},
};

static uint8_t EEMEM ee_status;
static User EEMEM database[MAX_USERS];

void database_init(void) {
    uint8_t flag;
    flag = eeprom_read_byte(&ee_status);

    if (flag == MAGIC) {
        return;
    }

    for (uint16_t i = 0; i < INIT_USERS; i++) {
        const uint8_t *flash_ptr = (const uint8_t *)&init_database[i];
        uint8_t *eemem_ptr = (uint8_t *)&database[i];

        for (uint16_t j = 0; j < sizeof(User); j++) {
            uint8_t val;
            val = pgm_read_byte(flash_ptr + j);
            eeprom_update_byte(eemem_ptr + j, val);
        }
    }

    eeprom_update_byte(&ee_status, MAGIC);
}

void database_erase(void) {
    eeprom_update_byte(&ee_status, EE_EMPTY);

    for (uint16_t i = 0; i < MAX_USERS; i++) {
        for (uint16_t j = 0; j < BLAKE2S_OUTLEN; j++) {
            eeprom_update_byte((uint8_t *)&database[i].hash[j], EE_EMPTY);
        }
    }
}

error_t database_add_user(const uint8_t *id, const char name[static NAME_LEN]) {
    REQUIRE_NON_NULL(id);

    int8_t free_idx = -1;
    uint8_t tg_hash_buf[BLAKE2S_OUTLEN];

    blake2s(tg_hash_buf, id, RDM6300_RFID_LEN);

    for (uint16_t i = 0; i < MAX_USERS; i++) {
        bool match = true, empty = true;

        for (uint16_t j = 0; j < BLAKE2S_OUTLEN; j++) {
            uint8_t val;
            val = eeprom_read_byte((const uint8_t *)&database[i].hash[j]);

            if (val != tg_hash_buf[j]) {
                match = false;
            }

            if (val != EE_EMPTY) {
                empty = false;
            }

            if (!match && !empty) {
                break;
            }
        }

        REQUIRE(!match, ERR_USER_ALREADY_EXISTS);

        if (free_idx == -1 && empty) {
            free_idx = (int8_t)i;
        }
    }

    REQUIRE(free_idx != -1, ERR_USER_DATABASE_IS_FULL);
    eeprom_update_block(tg_hash_buf, &database[free_idx].hash, BLAKE2S_OUTLEN);

    char name_buf[NAME_LEN] = {0};
    strncpy(name_buf, name, NAME_DATA_LEN);

    name_buf[NAME_DATA_LEN] = '\0';
    eeprom_update_block(name_buf, &database[free_idx].name, NAME_LEN);

    return ERR_NONE;
}

error_t database_find_user(const uint8_t *id, int8_t *ext_idx) {
    REQUIRE_NON_NULL(id);
    REQUIRE_NON_NULL(ext_idx);

    uint8_t tg_hash_buf[BLAKE2S_OUTLEN];
    blake2s(tg_hash_buf, id, RDM6300_RFID_LEN);

    for (uint16_t i = 0; i < MAX_USERS; i++) {
        bool match = true;

        for (uint16_t j = 0; j < BLAKE2S_OUTLEN; j++) {
            uint8_t val;
            val = eeprom_read_byte((const uint8_t *)&database[i].hash[j]);

            if (val != tg_hash_buf[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            *ext_idx = (int8_t)i;
            return ERR_NONE;
        }
    }

    return ERR_USER_NOT_FOUND;
}

error_t database_get_user_name(char *dest, size_t len, int8_t idx) {
    REQUIRE_NON_NULL(dest);
    REQUIRE(len > 2, ERR_SMALL_BUF);
    REQUIRE(idx >= 0 && idx < MAX_USERS, ERR_INVALID_INDEX);

    size_t n = (len < NAME_LEN) ? (len - 1) : (NAME_DATA_LEN);

    eeprom_read_block((void *)dest, &database[idx].name, n);
    dest[n] = '\0';

    return ERR_NONE;
}
