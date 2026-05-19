#ifndef USART_H
#define USART_H

#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 8000000UL

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
#define UART_BIT_SZ0 UCSZ0
#define UART_BIT_SZ1 UCSZ1
#define UART_SEL_REG (1 << URSEL)

#define BUF_SIZE 20

void uart_init(uint32_t baud);

extern volatile char rx_buffer[];
extern volatile uint8_t data_ready;
extern volatile uint8_t rx_index;

#endif
