#define F_CPU 8000000UL
#define loop while (1)

#include "database.h"
#include "uart.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include <stdio.h>

int main(void) {
    uart_init(9600);
    sei();
    stdout = &uart_output;
    eeprom_db_init();

    loop {}

    return 0;
}
