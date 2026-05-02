#include "error.h"

#define STRINGIFY(name) #name
#define STRINGIFY_CASE(name)                                                   \
    case name:                                                                 \
        return STRINGIFY(name);

const char *err_name(error_t err) {
    switch (err) {
        STRINGIFY_CASE(ERR_NONE)
        STRINGIFY_CASE(ERR_GENERIC)
        STRINGIFY_CASE(ERR_NULLPTR)
        STRINGIFY_CASE(ERR_SMALLBUF)
    default:
        return "ERR_UNKNOWN";
    }
}
