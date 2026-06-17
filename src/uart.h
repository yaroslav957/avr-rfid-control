#ifndef USART_H
#define USART_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

#define F_CPU 8000000UL
#define BUF_SIZE 10

#define UART_STATUS UCSRA
#define UART_CONTROL UCSRB
#define UART_DATA UDR
#define UART_BAUD_H UBRRH
#define UART_BAUD_L UBRRL
#define UART_CONFIG UCSRC
#define UART_READY (1 << UDRE)
#define UART_TXEN (1 << TXEN)
#define UART_RXEN (1 << RXEN)
#define UART_RXCIE (1 << RXCIE)
#define UART_BIT_SZ0 (1 << UCSZ0)
#define UART_BIT_SZ1 (1 << UCSZ1)
#define UART_SEL_REG (1 << URSEL)

void uart_init(uint32_t baud);

extern volatile uint8_t rx_buf[];
extern volatile uint8_t buf_ready;
extern volatile uint8_t rx_idx;

#endif
