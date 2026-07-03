#include "usart.h"

#include <assert.h>
#include <avr/interrupt.h>

static_assert(PACKET_LEN == 12,
              "RDM6300 packet requires 10 data bytes + 2 checksum");

static volatile uint8_t rx_idx = 0;
static volatile uint8_t rx_ready = 0;
static volatile uint8_t rx_buf[PACKET_BUF_LEN];

void usart_init(void) {
    const uint16_t ubrr_value = (uint16_t)((F_CPU / (16UL * BAUD)) - 1);

    USART_BAUD_H = (uint8_t)((ubrr_value >> 8) & 0x0F);
    USART_BAUD_L = (uint8_t)ubrr_value;

    USART_CONTROL = USART_RXCIE | USART_RXEN | USART_TXEN;
    USART_CONFIG = USART_SEL_REG | USART_BIT_SZ1 | USART_BIT_SZ0;
}

void usart_buf_clear(void) {
    const uint8_t sreg = SREG;

    cli();

    rx_ready = 0;
    rx_idx = 0;
    SREG = sreg;
}

/* without inline :(
 *
 * usart_buf_ready:
 * .L__stack_usage = 0
 *      lds r25,rx_ready
 *      ldi r24,lo8(1)
 *      cpi r25,lo8(1)
 *      breq .L2
 *      ldi r24,0
 * .L2:
 *      ret
 */
bool usart_buf_ready(void) { return (rx_ready == 1); }

error_t usart_buf_get_id(uint8_t *restrict dest) {
    REQUIRE_NON_NULL(dest);
    REQUIRE(rx_ready, ERR_BUF_IS_BUSY);

    for (uint16_t i = 0; i < PACKET_ID_LEN; i++) {
        dest[i] = rx_buf[i];
    }

    dest[PACKET_ID_LEN] = '\0';

    return ERR_NONE;
}

ISR(USART_RXC_vect) {
    uint8_t idx = rx_idx;
    const uint8_t received = USART_DATA;

    if (rx_ready)
        return;

    if (received == PACKET_START) {
        idx = 0;
    } else if (received == PACKET_STOP) {
        if (idx == PACKET_LEN) {
            rx_buf[PACKET_ID_LEN] = '\0';
            rx_ready = 1;
        }

        idx = 0;
    } else {
        if (idx < PACKET_ID_LEN) {
            rx_buf[idx] = received;
        }

        if (idx < PACKET_LEN) {
            idx++;
        }
    }

    rx_idx = idx;
}
