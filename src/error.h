#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERR_NONE = 0,

    ERR_NULL_PTR,
    ERR_SMALL_BUF,
    ERR_BUF_IS_BUSY,
    ERR_INVALID_IDX,

    ERR_USER_NOT_FOUND,
    ERR_USER_ALREADY_EXISTS,
    ERR_USER_DATABASE_IS_FULL,
} __attribute__((packed)) error_t;

#define REQUIRE(cond, err_code)                                                \
    do {                                                                       \
        if (!(cond)) {                                                         \
            return (err_code);                                                 \
        }                                                                      \
    } while (0)

#define REQUIRE_NON_NULL(ptr) REQUIRE((ptr) != NULL, ERR_NULL_PTR)

const char *err_name(error_t err_code);

#endif
