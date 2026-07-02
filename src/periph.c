#include "periph.h"

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
