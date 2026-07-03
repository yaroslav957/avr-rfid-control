#include "error.h"

#define STRINGIFY(name) #name
#define STRINGIFY_CASE(name)                                                   \
    case name:                                                                 \
        return STRINGIFY(name);

const char *err_name(error_t err) {
    switch (err) {
        STRINGIFY_CASE(ERR_NONE)
        STRINGIFY_CASE(ERR_NULL_PTR)
        STRINGIFY_CASE(ERR_SMALL_BUF)
        STRINGIFY_CASE(ERR_BUF_IS_BUSY)
        STRINGIFY_CASE(ERR_INVALID_INDEX)
        STRINGIFY_CASE(ERR_USER_NOT_FOUND)
        STRINGIFY_CASE(ERR_USER_ALREADY_EXISTS)
        STRINGIFY_CASE(ERR_USER_DATABASE_IS_FULL)
    default:
        return "ERR_UNKNOWN";
    }
}
