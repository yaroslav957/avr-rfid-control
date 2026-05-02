#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERR_NONE = 0,
    ERR_GENERIC = -1,
    ERR_NULLPTR = -2,
    ERR_SMALLBUF = -3
} __attribute__((packed)) error_t;

const char *err_name(error_t err_code);

#endif
