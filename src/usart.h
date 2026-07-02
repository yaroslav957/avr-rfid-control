#ifndef USART_H
#define USART_H

#include "error.h"

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#define BAUD 9600

#define PACKET_LEN 12
#define PACKET_ID_LEN 10
#define PACKET_BUF_LEN 11
#define PACKET_START 0x02
#define PACKET_STOP 0x03

#define USART_DATA UDR
#define USART_STATUS UCSRA
#define USART_CONTROL UCSRB
#define USART_BAUD_H UBRRH
#define USART_BAUD_L UBRRL
#define USART_CONFIG UCSRC
#define USART_READY (1 << UDRE)
#define USART_TXEN (1 << TXEN)
#define USART_RXEN (1 << RXEN)
#define USART_RXCIE (1 << RXCIE)
#define USART_BIT_SZ0 (1 << UCSZ0)
#define USART_BIT_SZ1 (1 << UCSZ1)
#define USART_SEL_REG (1 << URSEL)

void usart_init();
void usart_buf_clear(void);
bool usart_buf_ready(void);
error_t usart_buf_get_id(uint8_t *restrict dest);

#endif
