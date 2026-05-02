#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <stdio.h>

#if defined(__AVR_ATmega2560__)
#    define UART_STATUS UCSR0A
#    define UART_CONTROL UCSR0B
#    define UART_DATA UDR0
#    define UART_BAUD_H UBRR0H
#    define UART_BAUD_L UBRR0L
#    define UART_CONFIG UCSR0C
#    define UART_READY (1 << UDRE0)
#    define UART_TXEN (1 << TXEN0)
#    define UART_RXEN (1 << RXEN0)
#    define UART_RXCIE (1 << RXCIE0)
//   Frame format bits
#    define UART_BIT_SZ0 UCSZ00
#    define UART_BIT_SZ1 UCSZ01
#    define UART_SEL_REG 0
#else
#    define UART_STATUS UCSRA
#    define UART_CONTROL UCSRB
#    define UART_DATA UDR
#    define UART_BAUD_H UBRRH
#    define UART_BAUD_L UBRRL
#    define UART_CONFIG UCSRC
#    define UART_READY (1 << UDRE)
#    define UART_TXEN (1 << TXEN)
#    define UART_RXEN (1 << RXEN)
#    define UART_RXCIE (1 << RXCIE)
//   Frame format bits
#    define UART_BIT_SZ0 UCSZ0
#    define UART_BIT_SZ1 UCSZ1
#    define UART_SEL_REG (1 << URSEL)
#endif

#define BUF_SIZE 20

void uart_init(uint32_t baud);
int uart_putchar(char c, FILE *stream);

extern volatile char rx_buffer[];
extern volatile uint8_t data_ready;
extern volatile uint8_t rx_index;
extern FILE uart_output;

#endif
