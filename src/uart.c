#include "uart.h"
#include <stdint.h>

volatile uint8_t rx_buf[BUF_SIZE];
volatile uint8_t buf_ready = 0;
volatile uint8_t rx_idx = 0;

void uart_init(uint32_t baud) {
    uint16_t ubrr_value = (F_CPU / (16UL * baud)) - 1;

    UART_BAUD_H = (uint8_t)(ubrr_value >> 8);
    UART_BAUD_L = (uint8_t)ubrr_value;

    UART_CONTROL = UART_RXCIE | UART_RXEN | UART_TXEN;
    UART_CONFIG = UART_SEL_REG | UART_BIT_SZ1 | UART_BIT_SZ0;
}

ISR(USART_RXC_vect) {
    uint8_t received = UART_DATA;

    if (received == 0x02) {
        rx_idx = 0;
        buf_ready = 0;
    } else if (received == 0x03) {
        rx_buf[rx_idx] = '\0';
        buf_ready = 1;
    } else {
        if (rx_idx < BUF_SIZE - 1) {
            rx_buf[rx_idx++] = received;
        }
    }
}
