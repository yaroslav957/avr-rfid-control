#ifndef TIMERS_H
#define TIMERS_H

#include <avr/io.h>

#define TIMER0_DIVIDER 10

#define TIMER_MASK TIMSK
#define TIMER_FLAGS TIFR

#define TIMER0_COUNTER TCNT0
#define TIMER0_COMPARE OCR0
#define TIMER0_CONTROL TCCR0
#define TIMER0_CTC (1 << WGM01)
#define TIMER0_PRESC_256 (1 << CS02)
#define TIMER0_MATCH_IE (1 << OCIE0)
#define TIMER0_MATCH_FLAG (1 << OCF0)
#define TIMER0_OVF_FLAG (1 << TOV0)

#define TIMER1_COUNTER TCNT1
#define TIMER1_COMPARE_A OCR1A
#define TIMER1_CONTROL_A TCCR1A
#define TIMER1_CONTROL_B TCCR1B
#define TIMER1_OC1A_CLEAR (1 << COM1A1)
#define TIMER1_WGM_LOW (1 << WGM10)
#define TIMER1_PRESC_64 (1 << CS11)

void timer0_init(void);
void timer1_init(void);

#endif
