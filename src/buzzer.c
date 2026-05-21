#include "buzzer.h"

void buzzer_init(uint32_t frequency) {
    DDRB_REG |= (1 << BUZZER_PIN);

    BUZZER_TIMER_CONTROL = BUZZER_CTC_MODE | BUZZER_PRESCALER_64;
    BUZZER_COMPARE_VAL = (F_CPU / (2UL * PRESCALER * frequency)) - 1;
}

void buzzer_beep(void) {
    BUZZER_TIMER_CONTROL |= BUZZER_ON_BIT;

    _delay_ms(DELAY);

    BUZZER_TIMER_CONTROL &= ~BUZZER_ON_BIT;
    PORTB_REG &= ~(1 << BUZZER_PIN);
}
