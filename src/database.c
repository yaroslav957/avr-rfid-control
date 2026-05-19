#include "database.h"
#include "error.h"

const User PROGMEM __INIT[INIT_USERS] = {
    // "1234567890"
    {.hash = {0x36, 0x9d, 0x27, 0x0b, 0x22, 0x97, 0x1d, 0x73, 0xd5, 0xc2, 0xba,
              0x4e, 0x95, 0x6a, 0x9f, 0x02, 0x11, 0x0e, 0x56, 0x91, 0x7e, 0x11,
              0x83, 0x3e, 0x61, 0xb7, 0x0e, 0x97, 0x33, 0x4a, 0x6d, 0x20},
     .name = "Admin"}};

uint8_t EEMEM ee_status;
User EEMEM database[MAX_USERS];

void eeprom_db_init(void) {
    uint8_t flag;
    flag = eeprom_read_byte(&ee_status);

    if (flag == MAGIC)
        return;

    for (uint8_t i = 0; i < INIT_USERS; i++) {
        User user;
        memcpy_P(&user, &__INIT[i], sizeof(User));
        eeprom_write_block(&user, &database[i], sizeof(User));
    }

    eeprom_update_byte(&ee_status, MAGIC);
}

error_t add_user(const char *id, char name[NAME_LEN]) {
    ;
    return ERR_NONE;
}

error_t find_user(const char *id, int8_t *ext_idx) {
    if (id == NULL || ext_idx == NULL)
        return ERR_NULL_PTR;

    uint8_t target_hash[BLAKE2S_OUTLEN];
    uint8_t db_hash_buf[BLAKE2S_OUTLEN];

    blake2s(target_hash, (const uint8_t *)id, RDM6300_RFID_LEN);

    for (uint8_t i = 0; i < MAX_USERS; i++) {
        eeprom_read_block(db_hash_buf, &database[i].hash, BLAKE2S_OUTLEN);

        if (memcmp(target_hash, db_hash_buf, BLAKE2S_OUTLEN) == 0) {
            *ext_idx = i;
            return ERR_NONE;
        }
    }

    return ERR_NOTFOUND;
}

error_t get_user_name(char *dest, size_t len, int8_t idx) {
    if (dest == NULL)
        return ERR_NULL_PTR;

    if (len < 2)
        return ERR_SMALLBUF;

    if (idx < 0 || idx >= MAX_USERS)
        return ERR_INVALIDX;

    size_t n = (len < NAME_LEN) ? (len - 1) : (NAME_LEN - 1);
    eeprom_read_block((void *)dest, &database[idx].name, n);
    dest[n] = '\0';

    return ERR_NONE;
}
