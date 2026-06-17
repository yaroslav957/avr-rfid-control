#include "database.h"

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include <assert.h>
#include <string.h>

const User PROGMEM init_database[INIT_USERS] = {
    {.hash = {0x84, 0xE5, 0xA2, 0x9B, 0x07, 0x1D, 0x8C, 0x67, 0x5D, 0x2A, 0x11,
              0xEB, 0xEE, 0x22, 0x53, 0x13, 0x6A, 0x74, 0x26, 0x5A, 0xE4, 0x00,
              0x5A, 0x3D, 0xD6, 0x4F, 0xAE, 0x0E, 0x4A, 0xBC, 0xEF, 0x98},
     .name = "Admin"},
};

static_assert(INIT_USERS <= MAX_USERS,
              "Flash database can't be bigger than EEPROM");

uint8_t EEMEM ee_status;
User EEMEM database[MAX_USERS];

void eeprom_db_init(void) {
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

void eeprom_db_erase(void) {
    User user = {0};

    eeprom_update_byte(&ee_status, 0xFF);
    for (uint8_t i = 0; i < MAX_USERS; i++) {
        eeprom_update_block(&user, &database[i], sizeof(User));
    }
}

error_t add_user(const uint8_t *id, const char *name) {
    RETURN_IF_NULL(id);
    RETURN_IF_NULL(name);

    User user;
    int8_t idx = -1;
    uint8_t tg_hash_buf[BLAKE2S_OUTLEN];
    uint8_t db_hash_buf[BLAKE2S_OUTLEN];

    blake2s(tg_hash_buf, id, RDM6300_RFID_LEN);

    for (uint8_t i = 0; i < MAX_USERS; i++) {
        eeprom_read_block(db_hash_buf, &database[i].hash, BLAKE2S_OUTLEN);
        if (memcmp(tg_hash_buf, db_hash_buf, BLAKE2S_OUTLEN) == 0) {
            return ERR_USER_ALREADY_EXISTS;
        }

        if (idx == -1 && db_hash_buf[0] == 0x00) {
            idx = i;
        }
    }

    if (idx == -1) {
        return ERR_USER_DATABASE_IS_FULL;
    }

    memcpy(user.hash, tg_hash_buf, BLAKE2S_OUTLEN);
    strncpy(user.name, name, NAME_DATA_LEN);
    user.name[NAME_DATA_LEN] = '\0';
    eeprom_update_block(&user, &database[idx], sizeof(User));

    return ERR_NONE;
}

error_t find_user(const uint8_t *id, int8_t *ext_idx) {
    RETURN_IF_NULL(id);
    RETURN_IF_NULL(ext_idx);

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

error_t get_user_name(char *dest, size_t len, int8_t idx) {
    RETURN_IF_NULL(dest);

    if (len < 2)
        return ERR_SMALL_BUF;
    if (idx < 0 || idx >= MAX_USERS)
        return ERR_INVALID_IDX;

    size_t n = (len < NAME_LEN) ? (len - 1) : (NAME_LEN - 1);

    eeprom_read_block((void *)dest, &database[idx].name, n);
    dest[n] = '\0';

    return ERR_NONE;
}
