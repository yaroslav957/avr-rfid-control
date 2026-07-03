#ifndef PERIPH_H
#define PERIPH_H

#include <avr/io.h>
#include <stdint.h>

#define ALL_LOW 0x00
#define ALL_OUT 0xFF

#define DDR_PORT_A DDRA
#define DDR_PORT_B DDRB
#define DDR_PORT_C DDRC
#define DDR_PORT_D DDRD
#define PORT_A PORTA
#define PORT_B PORTB
#define PORT_D PORTD
#define PORT_A_BIT2 (1 << PA2)
#define PORT_A_BIT3 (1 << PA3)
#define PORT_B_BIT0 (1 << PB0)
#define PORT_B_BIT1 (1 << PB1)
#define PORT_B_BIT2 (1 << PB2)
#define PORT_B_BIT5 (1 << PB5)
#define PORT_B_BIT6 (1 << PB6)
#define PORT_B_BIT7 (1 << PB7)
#define PORT_D_BIT5 (1 << PD5)
#define PORT_D_BIT6 (1 << PD6)
#define PORT_D_BIT7 (1 << PD7)

#define ADC_MUX ADMUX
#define ADC_CONTROL ADCSRA
#define ADC_DATA ADC
#define ADC_AREF 0x00
#define ADC_CHANNEL_0 0x00
#define ADC_CHANNEL_1 0x01
#define ADC_ENABLE (1 << ADEN)
#define ADC_START (1 << ADSC)
#define ADC_PRESC_32 ((1 << ADPS2) | (1 << ADPS0))

void gpio_init(void);
void adc_init(void);
void adc_update_pos(void);
void adc_read_pos(uint16_t *ext_x, uint16_t *ext_y);

#endif
