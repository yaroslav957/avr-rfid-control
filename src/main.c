#define F_CPU 8000000UL

#include "database.h"
#include "uart.h"

#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

void wait(unsigned long ticks);

int main(void) {
    uart_init(9600);
    eeprom_db_init();
    sei();

    while (1) {
        wait(5);
    }

    return EXIT_SUCCESS;
}

void wait(uint32_t ticks) {
    for (int32_t i = 0; i < ticks; i++) {
        asm volatile("nop");
    }
}
