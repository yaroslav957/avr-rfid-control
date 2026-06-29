#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

static inline void wait(uint32_t ticks) {
    for (uint32_t i = 0; i < ticks; i++) {
        __asm__ __volatile__("nop");
    }
}

#endif
