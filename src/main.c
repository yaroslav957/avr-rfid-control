#include "buzzer.h"
#include "database.h"
#include "error.h"
#include "periph.h"
#include "timers.h"
#include "usart.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <glcd.h>
#include <util/delay.h>

static void scan_key(void);
static void draw_main(void);
static void draw_admin(void);

volatile uint8_t gkey = 0;
volatile uint8_t gkey_status = 0;
volatile uint8_t adding = 0;
volatile uint8_t admin_mode = 0;
volatile uint8_t found_user = 0;

static char user_name[NAME_LEN];

int main(void) {
    cli();

    gpio_init();
    usart_init();
    buzzer_init();
    database_init();

    _delay_ms(100);
    glcd_off();
    _delay_ms(100);

    glcd_on();
    glcd_clear();
    timer1_init();
    timer0_cb(scan_key);
    timer0_init();
    adc_init();

    draw_main();
    sei();

    while (1) {
        if (usart_buf_ready()) {
            uint8_t rfid[PACKET_BUF_LEN];

            if (usart_buf_get_id(rfid) == ERR_NONE) {
                if (!adding) {
                    int8_t idx = -1;

                    if (database_find_user(rfid, &idx) == ERR_NONE &&
                        idx >= 0) {
                        found_user = 1;
                        if (database_get_user_name(user_name, sizeof(user_name),
                                                   idx) != ERR_NONE) {
                            found_user = 0;
                        }
                    } else {
                        glcd_clear();
                        glcd_puts("Unknown RFID", 16, 3, 0, 1, 0);
                        buzzer_beep();
                        admin_mode = 0;
                        found_user = 0;
                    }

                    draw_main();
                } else {
                    const char new_name[NAME_LEN] = "NEW_USER";
                    error_t add_result = database_add_user(rfid, new_name);

                    glcd_clear();
                    if (add_result == ERR_NONE) {
                        glcd_puts("User added", 24, 3, 0, 1, 0);
                    } else {
                        glcd_puts("User exists", 22, 3, 0, 1, 0);
                    }

                    adding = 0;
                    admin_mode = 0;
                    found_user = 0;

                    buzzer_beep();
                    draw_main();
                }
            }

            usart_buf_clear();
        }

        if ((gkey_status & 0b10000000) != 0) {
            gkey_status = 0;

            if (admin_mode) {
                if (gkey == 1) {
                    glcd_puts("Erased   ", 10, 3, 0, 1, 0);
                    database_erase();
                    admin_mode = 0;
                    found_user = 0;
                    draw_main();
                }
                if (gkey == 2) {
                    glcd_puts("Scan card", 10, 3, 0, 1, 0);
                    adding = 1;
                    usart_buf_clear();
                }
                if (gkey == 3) {
                    admin_mode = 0;
                    found_user = 0;
                    adding = 0;
                    draw_main();
                }
            } else {
                if (gkey == 0 && found_user) {
                    admin_mode = 1;
                    draw_admin();
                }
            }
        }
    }
}

static void draw_main(void) {
    glcd_clear();

    if (found_user) {
        glcd_puts("Authorized user", 2, 1, 0, 1, 0);
        glcd_puts(">", 1, 3, 0, 1, 0);
        glcd_puts(user_name, 9, 3, 0, 1, 0);
        rectangle(0, 40, 127, 63, 0, 1);
        glcd_puts("ADMIN", 44, 6, 0, 1, 0);
    } else {
        glcd_puts("Press card", 24, 3, 0, 1, 0);
    }
}

static void draw_admin(void) {
    glcd_clear();
    glcd_puts("Admin mode", 2, 1, 0, 1, 0);
    glcd_puts(">", 1, 3, 0, 1, 0);
    rectangle(0, 40, 44, 63, 0, 1);
    glcd_puts("Del", 10, 6, 0, 1, 1);
    rectangle(44, 40, 83, 63, 0, 1);
    glcd_puts("Add", 52, 6, 0, 1, 1);
    rectangle(83, 40, 127, 63, 0, 1);
    glcd_puts("Exit", 89, 6, 0, 1, 1);
}

static void scan_key(void) {
    uint16_t adc_x;
    uint16_t adc_y;

    PORTA = 0b00000100;
    _delay_ms(2);
    ADMUX = 0b00000000;
    ADCSRA |= (1 << ADSC);
    while ((ADCSRA & (1 << ADSC)) != 0)
        ;
    adc_x = ADC;

    PORTA = 0b00001000;
    _delay_ms(2);
    ADMUX = 0b00000001;
    ADCSRA |= (1 << ADSC);
    while ((ADCSRA & (1 << ADSC)) != 0)
        ;
    adc_y = ADC;

    if (admin_mode == 0) {
        if (((adc_y > 200) && (adc_y < 1020)) && (gkey_status == 0)) {
            gkey_status = 0b11000000;
            gkey = 0;
        }
    } else {
        if (((adc_y > 200) && (adc_y < 1020)) &&
            ((adc_x > 30) && (adc_x < 180)) && (gkey_status == 0)) {
            gkey_status = 0b11000000;
            gkey = 1;
        }
        if (((adc_y > 200) && (adc_y < 1020)) &&
            ((adc_x > 300) && (adc_x < 480)) && (gkey_status == 0)) {
            gkey_status = 0b11000000;
            gkey = 2;
        }
        if (((adc_y > 200) && (adc_y < 1020)) &&
            ((adc_x > 580) && (adc_x < 730)) && (gkey_status == 0)) {
            gkey_status = 0b11000000;
            gkey = 3;
        }
    }
}
