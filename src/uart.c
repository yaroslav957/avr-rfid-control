#include "uart.h"

volatile char rx_buffer[BUF_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t data_ready = 0;

void uart_init(uint32_t baud) {
    uint16_t ubrr_value = (F_CPU / (16UL * baud)) - 1;

    UART_BAUD_H = (uint8_t)(ubrr_value >> 8);
    UART_BAUD_L = (uint8_t)ubrr_value;

    UART_CONTROL = UART_RXCIE | UART_RXEN | UART_TXEN;
    UART_CONFIG = UART_SEL_REG | (1 << UART_BIT_SZ1) | (1 << UART_BIT_SZ0);
}

ISR(USART_RX_vect) {
    char received = UDR;

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
