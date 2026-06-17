#ifndef BUZZER_H
#define BUZZER_H

#include <avr/io.h>

#define F_BUZZER 3000U
#define HALF_PERIOD 166

#define BUZZER_DDR DDRB
#define BUZZER_PORT PORTB
#define BUZZER_PIN PB1

void buzzer_init(void);
void buzzer_beep(void);

#endif
