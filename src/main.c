#define F_CPU 8000000UL

#include "buzzer.h"
#include "database.h"
#include "uart.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void wait(uint32_t ticks);

int main(void) {
    cli();

    uart_init(9600);
    buzzer_init(1000);
    eeprom_db_init();

    sei();

    while (true) {
        wait(5);
    }

    return EXIT_SUCCESS;
}

void wait(uint32_t ticks) {
    for (int32_t i = 0; i < ticks; i++) {
        asm volatile("nop");
    }
}
