#include "periph.h"

#include <util/atomic.h>
#include <util/delay.h>

static volatile uint16_t adc_x = 0;
static volatile uint16_t adc_y = 0;

void gpio_init(void) {
    PORT_A = PORT_A_BIT2;
    DDR_PORT_A = PORT_A_BIT2 | PORT_A_BIT3;
    PORT_B = ALL_LOW;
    DDR_PORT_B = PORT_B_BIT0 | PORT_B_BIT1 | PORT_B_BIT2 | PORT_B_BIT5 |
                 PORT_B_BIT6 | PORT_B_BIT7;
    PORT_D = PORT_D_BIT7;
    DDR_PORT_D = PORT_D_BIT5 | PORT_D_BIT6 | PORT_D_BIT7;
    DDR_PORT_C = ALL_OUT;
}

void adc_init(void) {
    ADC_MUX = ADC_AREF;
    ADC_CONTROL = ADC_ENABLE | ADC_PRESC_32;
}

static uint16_t adc_get_pos(uint8_t port, uint8_t channel) {
    PORT_A = port;
    _delay_ms(2);
    ADC_MUX = ADC_AREF | channel;
    ADC_CONTROL |= ADC_START;

    while ((ADC_CONTROL & ADC_START) != 0)
        ;
    return ADC_DATA;
}

void adc_update_pos(void) {
    adc_x = adc_get_pos(PORT_A_BIT2, ADC_CHANNEL_0);
    adc_y = adc_get_pos(PORT_A_BIT3, ADC_CHANNEL_1);
}

void adc_read_pos(uint16_t *ext_x, uint16_t *ext_y) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        *ext_x = adc_x;
        *ext_y = adc_y;
    }
}
