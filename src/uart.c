#define F_CPU 8000000UL

#include "uart.h"
#include <avr/interrupt.h>

volatile char rx_buffer[BUF_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t data_ready = 0;

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void uart_init(uint32_t baud) {
    uint16_t ubrr_value = (F_CPU / (16UL * baud)) - 1;

    UART_BAUD_H = (uint8_t)(ubrr_value >> 8);
    UART_BAUD_L = (uint8_t)ubrr_value;

    UART_CONTROL = UART_RXCIE | UART_RXEN | UART_TXEN;
    UART_CONFIG = UART_SEL_REG | (1 << UART_BIT_SZ1) | (1 << UART_BIT_SZ0);
}

int uart_putchar(char c, FILE *stream) {
    if (c == '\n')
        uart_putchar('\r', stream);
    while (!(UART_STATUS & UART_READY))
        ;
    UART_DATA = c;

    return 0;
}

ISR(USART0_RX_vect) {
    char received = UDR0;

    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = received;

    if (received == '\r' || received == '\n') {
        if (rx_index > 0) {
            rx_buffer[rx_index] = '\0';
            data_ready = 1;
            rx_index = 0;
        }
    } else {
        if (data_ready == 1) {
            data_ready = 0;
            rx_index = 0;
        }

        if (rx_index < BUF_SIZE - 1) {
            if (received != ' ') {
                rx_buffer[rx_index++] = received;
            }
        }
    }
}

/*
ISR(USART0_RX_vect) {
    char received = UDR0;

    if (received == 0x02) {
        rx_index = 0;
        data_ready = 0;
    } else if (received == 0x03) {
        rx_buffer[rx_index] = '\0';
        data_ready = 1;
    } else {
        if (rx_index < BUF_SIZE - 1) {
            rx_buffer[rx_index++] = received;
        }
    }
}
*/
