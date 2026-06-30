#ifndef DATABASE_H
#define DATABASE_H

#include "error.h"

#include <blake2s/blake2s.h>
#include <stdint.h>

#define MAGIC 0xDB
#define EE_EMPTY 0xFF

#define INIT_USERS 1
#define MAX_USERS 16

#define RDM6300_RFID_LEN 10
#define NAME_DATA_LEN 15
#define NAME_LEN (NAME_DATA_LEN + 1)

typedef struct {
    uint8_t hash[BLAKE2S_OUTLEN];
    char name[NAME_LEN];
} User;

void database_init(void);
void database_erase(void);
error_t database_find_user(const uint8_t *id, int8_t *ext_idx);
error_t database_add_user(const uint8_t *id, const char name[static NAME_LEN]);
error_t database_get_user_name(char *dest, size_t len, int8_t idx);

#endif
