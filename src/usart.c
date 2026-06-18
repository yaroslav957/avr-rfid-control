#include "error.h"
#include "usart.h"

#include <avr/interrupt.h>

#include <string.h>

static volatile uint8_t rx_idx = 0;
static volatile uint8_t rx_ready = 0;
static volatile uint8_t rx_buf[PACKET_BUF_LEN];

void uart_init(uint32_t baud) {
    uint16_t ubrr_value = (F_CPU / (16UL * baud)) - 1;

    USART_BAUD_H = (uint8_t)(ubrr_value >> 8);
    USART_BAUD_L = (uint8_t)ubrr_value;
    USART_CONTROL = USART_RXCIE | USART_RXEN | USART_TXEN;
    USART_CONFIG = USART_SEL_REG | USART_BIT_SZ1 | USART_BIT_SZ0;
}

void uart_buf_clear(void) {
    uint8_t sreg = SREG;

    cli();

    rx_ready = 0;
    rx_idx = 0;
    SREG = sreg;
}

error_t uart_buf_ready(void) {
    REQUIRE(rx_ready == 1, ERR_BUF_IS_BUSY);

    return ERR_NONE;
}

error_t uart_buf_get_id(char *dest) {
    REQUIRE_NON_NULL(dest);
    REQUIRE(rx_ready, ERR_BUF_IS_BUSY);

    memcpy(dest, (const char *)rx_buf, 10);
    dest[10] = '\0';

    return ERR_NONE;
}

ISR(USART_RXC_vect) {
    unsigned char received = USART_DATA;

    if (rx_ready)
        return;

    if (received == PACKET_START) {
        rx_idx = 0;
    } else if (received == PACKET_STOP) {
        if (rx_idx == PACKET_LEN) {
            rx_buf[rx_idx] = '\0';
            rx_ready = 1;
        } else {
            rx_idx = 0;
        }
    } else {
        if (rx_idx < PACKET_LEN) {
            rx_buf[rx_idx++] = received;
        }
    }
}
