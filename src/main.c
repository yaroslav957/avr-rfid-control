#define F_CPU 8000000UL
#define loop while (1)

#include "database.h"
#include "uart.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <stdio.h>

int main(void) {
    uart_init(9600);
    sei();
    stdout = &uart_output;
    eeprom_db_init();

    printf("--- RFID System Ready ---\n");

    loop {
        if (data_ready) {
            uint8_t user_idx;
            find_user((const char *)rx_buffer, &user_idx);

            if (user_idx != -1) {
                char current_name[NAME_LEN];
                get_user_name(current_name, 10, user_idx);

                printf("Access Granted! Welcome, %s\n", current_name);
            } else {
                printf("Access Denied! Unknown ID: '%s'\n", (char *)rx_buffer);
            }

            data_ready = 0;
            for (uint8_t i = 0; i < BUF_SIZE; i++)
                rx_buffer[i] = 0;
            rx_index = 0;
        }
    }

    return 0;
}
