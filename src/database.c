#include "database.h"
#include "error.h"

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include <assert.h>
#include <string.h>

static_assert(INIT_USERS <= MAX_USERS,
              "Flash database can't be bigger than EEPROM");

const User PROGMEM init_database[INIT_USERS] = {
    {.hash = {0x84, 0xE5, 0xA2, 0x9B, 0x07, 0x1D, 0x8C, 0x67, 0x5D, 0x2A, 0x11,
              0xEB, 0xEE, 0x22, 0x53, 0x13, 0x6A, 0x74, 0x26, 0x5A, 0xE4, 0x00,
              0x5A, 0x3D, 0xD6, 0x4F, 0xAE, 0x0E, 0x4A, 0xBC, 0xEF, 0x98},
     .name = "Admin"},
};

uint8_t EEMEM ee_status;
User EEMEM database[MAX_USERS];

void database_init(void) {
    uint8_t flag;
    flag = eeprom_read_byte(&ee_status);

    if (flag == MAGIC)
        return;

    for (uint8_t i = 0; i < INIT_USERS; i++) {
        User user;
        memcpy_P(&user, &init_database[i], sizeof(User));
        eeprom_write_block(&user, &database[i], sizeof(User));
    }

    eeprom_update_byte(&ee_status, MAGIC);
}

void database_erase(void) {
    User user = {0};

    eeprom_update_byte(&ee_status, 0xFF);
    for (uint8_t i = 0; i < MAX_USERS; i++) {
        eeprom_update_block(&user, &database[i], sizeof(User));
    }
}

error_t database_add_user(const uint8_t *id, const char name[NAME_LEN]) {
    REQUIRE_NON_NULL(id);
    REQUIRE_NON_NULL(name);

    User user;
    int8_t free_idx = -1;

    uint8_t tg_hash_buf[BLAKE2S_OUTLEN];
    uint8_t db_hash_buf[BLAKE2S_OUTLEN];

    uint8_t empty_zero[BLAKE2S_OUTLEN] = {0};
    uint8_t empty_ff[BLAKE2S_OUTLEN];

    memset(empty_ff, 0xFF, BLAKE2S_OUTLEN);
    blake2s(tg_hash_buf, id, RDM6300_RFID_LEN);

    for (uint8_t i = 0; i < MAX_USERS; i++) {
        eeprom_read_block(db_hash_buf, &database[i].hash, BLAKE2S_OUTLEN);

        if (memcmp(tg_hash_buf, db_hash_buf, BLAKE2S_OUTLEN) == 0) {
            return ERR_USER_ALREADY_EXISTS;
        }

        if (free_idx == -1) {
            if (memcmp(db_hash_buf, empty_zero, BLAKE2S_OUTLEN) == 0 ||
                memcmp(db_hash_buf, empty_ff, BLAKE2S_OUTLEN) == 0) {
                free_idx = i;
            }
        }
    }

    REQUIRE(free_idx != -1, ERR_USER_DATABASE_IS_FULL);

    memcpy(user.hash, tg_hash_buf, BLAKE2S_OUTLEN);
    strncpy(user.name, name, NAME_DATA_LEN);
    user.name[NAME_DATA_LEN] = '\0';
    eeprom_update_block(&user, &database[free_idx], sizeof(User));

    return ERR_NONE;
}

error_t database_find_user(const uint8_t *id, int8_t *ext_idx) {
    REQUIRE_NON_NULL(id);
    REQUIRE_NON_NULL(ext_idx);

    uint8_t tg_hash_buf[BLAKE2S_OUTLEN];
    uint8_t db_hash_buf[BLAKE2S_OUTLEN];

    blake2s(tg_hash_buf, id, RDM6300_RFID_LEN);

    for (uint8_t i = 0; i < MAX_USERS; i++) {
        eeprom_read_block(db_hash_buf, &database[i].hash, BLAKE2S_OUTLEN);
        if (memcmp(tg_hash_buf, db_hash_buf, BLAKE2S_OUTLEN) == 0) {
            *ext_idx = i;

            return ERR_NONE;
        }
    }

    return ERR_USER_NOT_FOUND;
}

error_t databse_get_user_name(char *dest, size_t len, int8_t idx) {
    REQUIRE_NON_NULL(dest);
    REQUIRE(len > 2, ERR_SMALL_BUF);
    REQUIRE(idx > 0 && idx <= MAX_USERS, ERR_INVALID_IDX);

    size_t n = (len < NAME_LEN) ? (len - 1) : (NAME_DATA_LEN);

    eeprom_read_block((void *)dest, &database[idx].name, n);
    dest[n] = '\0';

    return ERR_NONE;
}
