#include "database.h"
#include "error.h"

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include <stdint.h>
#include <string.h>

const User PROGMEM INIT[INIT_USERS] = {{"1234567890", "Admin"},
                                       {"0123456789", "KalugDron77"},
                                       {"0987654321", "Fateev T.S."},
                                       {"0192837465", "Kanaev S.A."},
                                       {"1234509876", "Vasin G.R."}};

uint8_t EEMEM ee_status;
User EEMEM database[MAX_USERS];

void eeprom_db_init(void) {
    uint8_t flag;
    flag = eeprom_read_byte((EEP_BYTE_P)&ee_status);

    if (flag == MAGIC)
        return;

    for (uint8_t i = 0; i < INIT_USERS; i++) {
        User user;
        memcpy_P(&user, &INIT[i], sizeof(User));
        eeprom_write_block(&user, &database[i], sizeof(User));
    }

    eeprom_update_byte(&ee_status, MAGIC);
}

error_t find_user(const char *id, uint8_t *ext_idx) {
    if (id == NULL || ext_idx == NULL)
        return ERR_NULLPTR;

    char buf[ID_LEN];
    for (uint8_t i = 0; i < MAX_USERS; i++) {
        eeprom_read_block((void *)buf, (EEP_VOID_P)&database[i].id, ID_LEN);
        buf[ID_DATA_LEN] = '\0';

        if (strcmp(id, buf) == 0) {
            *ext_idx = i;
            return ERR_NONE;
        }
    }

    return ERR_GENERIC;
}

error_t get_user_name(char *dest, size_t len, int8_t idx) {
    if (dest == NULL)
        return ERR_NULLPTR;

    if (len < 2)
        return ERR_SMALLBUF;

    if (idx < 0 || idx >= MAX_USERS)
        return ERR_GENERIC;

    size_t n = (len < NAME_LEN) ? (len - 1) : (NAME_LEN - 1);
    eeprom_read_block((void *)dest, (EEP_VOID_P)&database[idx].name, n);
    dest[n] = '\0';

    return ERR_NONE;
}
