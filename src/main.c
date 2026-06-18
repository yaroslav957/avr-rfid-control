#define F_CPU 8000000UL

#include "buzzer.h"
#include "database.h"
#include "usart.h"

#include <avr/interrupt.h>

#include <stdbool.h>
#include <stdint.h>

void wait(uint32_t ticks);

int main(void) {
    cli();

    uart_init(9600);
    buzzer_init();
    eeprom_db_init();

    sei();

    while (true) {
        wait(5);
    }
}

void wait(uint32_t ticks) {
    for (int32_t i = 0; i < ticks; i++) {
        asm volatile("nop");
    }
}
