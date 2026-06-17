#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERR_NONE = 0,

    ERR_NULL_PTR,
    ERR_SMALL_BUF,
    ERR_INVALID_IDX,

    ERR_USER_NOT_FOUND,
    ERR_USER_ALREADY_EXISTS,
    ERR_USER_DATABASE_IS_FULL,
} __attribute__((packed)) error_t;

#define RETURN_IF_NULL(ptr)                                                    \
    do {                                                                       \
        if ((ptr) == NULL) {                                                   \
            return ERR_NULL_PTR;                                               \
        }                                                                      \
    } while (0)

const char *err_name(error_t err_code);

#endif
