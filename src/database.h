#ifndef DATABASE_H
#define DATABASE_H

#include "error.h"

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <blake2s/blake2s.h>

#include <stdint.h>

#define MAGIC 0xDB

#define INIT_USERS 5
#define MAX_USERS 16

#define HASH_LEN 32
#define NAME_DATA_LEN 12
#define NAME_LEN (NAME_DATA_LEN + 1)

typedef const uint8_t *EEP_BYTE_P;
typedef const void *EEP_VOID_P;

typedef struct {
    uint8_t hash_id[HASH_LEN];
    char name[NAME_LEN];
} User;

void eeprom_db_init(void);
error_t find_user(const char *id, int8_t *ext_idx);
error_t get_user_name(char *dest, size_t len, int8_t idx);

#endif
