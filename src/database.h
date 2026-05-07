#ifndef DATABASE_H
#define DATABASE_H

#include "error.h"

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <blake2s/blake2s.h>

#include <stdint.h>
#include <string.h>

#define MAGIC 0xDB

#define INIT_USERS 5
#define MAX_USERS 16

#define RDM6300_RFID_LEN 11
#define NAME_DATA_LEN 15
#define NAME_LEN (NAME_DATA_LEN + 1)

typedef struct {
    uint8_t hash[BLAKE2S_OUTLEN];
    char name[NAME_LEN];
} User;

void eeprom_db_init(void);
error_t find_user(const char *id, int8_t *ext_idx);
error_t get_user_name(char *dest, size_t len, int8_t idx);

#endif
