#define F_CPU 8000000UL

#include "buzzer.h"
#include "database.h"
#include "usart.h"

#include <avr/interrupt.h>

int main(void) {
    cli();

    uart_init(9600);
    buzzer_init();
    database_init();

    sei();

    while (1) {
    }
}
