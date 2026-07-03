#include "drivers/periph.h"
#include "timers.h"

#include <avr/interrupt.h>

static volatile uint8_t cnt = 0;

void timer0_init(void) {
    TIMER0_COUNTER = 0;
    TIMER0_COMPARE = 249;
    TIMER0_CONTROL = TIMER0_CTC | TIMER0_PRESC_256;
    TIMER_MASK |= TIMER0_MATCH_IE;
    TIMER_FLAGS = TIMER0_MATCH_FLAG | TIMER0_OVF_FLAG;
}

void timer1_init(void) {
    TIMER1_COUNTER = 0;
    TIMER1_COMPARE_A = 200;
    TIMER1_CONTROL_A = TIMER1_OC1A_CLEAR | TIMER1_WGM_LOW;
    TIMER1_CONTROL_B = TIMER1_PRESC_64;
}

ISR(TIMER0_COMP_vect) {
    if (++cnt >= TIMER0_DIVIDER) {
        cnt = 0;
        adc_update_pos();
    }
}
