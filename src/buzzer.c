#include "buzzer.h"

#include <util/delay.h>

void buzzer_init(void) { BUZZER_DDR |= (1 << BUZZER_PIN); }

void buzzer_beep(void) {
    for (uint16_t i = 0; i < F_BUZZER; i++) {
        BUZZER_PORT |= (1 << BUZZER_PIN);
        _delay_us(HALF_PERIOD);

        BUZZER_PORT &= ~(1 << BUZZER_PIN);
        _delay_us(HALF_PERIOD);
    }
}
