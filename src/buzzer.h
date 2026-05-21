#ifndef BUZZER_H
#define BUZZER_H

#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>

#define F_CPU 8000000UL
#define PRESCALER 64
#define DELAY 500

#define DDRB_REG DDRB
#define PORTB_REG PORTB
#define BUZZER_PIN PB3
#define BUZZER_TIMER_CONTROL TCCR0
#define BUZZER_COMPARE_VAL OCR0
#define BUZZER_CTC_MODE (1 << WGM01)
#define BUZZER_PRESCALER_64 ((1 << CS01) | (1 << CS00))
#define BUZZER_ON_BIT (1 << COM00)

void buzzer_init(uint32_t frequency);
void buzzer_beep(void);

#endif
