#include "drivers/buzzer.h"
#include "drivers/periph.h"
#include "drivers/timers.h"
#include "drivers/usart.h"
#include "system/database.h"
#include "system/error.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <glcd.h>
#include <util/delay.h>

#define AREA_Y_MIN 200
#define AREA_Y_MAX 1023
#define IN_RANGE(v, lo, hi) ((v) > (lo) && (v) < (hi))

static char user_name[NAME_LEN];
static bool is_adding = false;
static bool is_key_held = false;
static bool is_admin_mode = false;
static bool is_user_found = false;

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
    timer0_init();
    adc_init();

    draw_main(is_user_found, user_name);
    sei();

    while (1) {
        if (usart_buf_ready()) {
            uint8_t rfid[PACKET_BUF_LEN];

            if (OK(usart_buf_get_id(rfid))) {
                if (!is_adding) {
                    int8_t idx = -1;

                    if (OK(database_find_user(rfid, &idx)) && idx >= 0) {
                        is_user_found = true;

                        if (!OK(database_get_user_name(
                                user_name, sizeof(user_name), idx))) {
                            is_user_found = false;
                        }
                    } else {
                        glcd_clear();
                        glcd_puts("Unknown RFID", 16, 3, 0, 1, 0);
                        buzzer_beep();
                        is_admin_mode = false;
                        is_user_found = false;
                    }

                    draw_main(is_user_found, user_name);
                } else {
                    glcd_clear();

                    if (OK(database_add_user(rfid, "NEW_USER"))) {
                        glcd_puts("User added", 24, 3, 0, 1, 0);
                    } else {
                        glcd_puts("User exists", 22, 3, 0, 1, 0);
                    }

                    is_adding = false;
                    is_admin_mode = false;
                    is_user_found = false;

                    buzzer_beep();
                    draw_main(is_user_found, user_name);
                }
            }

            usart_buf_clear();
        }

        uint16_t x, y;
        adc_read_pos(&x, &y);

        if (is_key_held) {
            if (!IN_RANGE(y, AREA_Y_MIN, AREA_Y_MAX)) {
                is_key_held = false;
            }
        } else if (IN_RANGE(y, AREA_Y_MIN, AREA_Y_MAX)) {
            is_key_held = true;

            if (!is_admin_mode) {
                if (is_user_found) {
                    is_admin_mode = true;
                    draw_admin();
                }
            } else if (IN_RANGE(x, 30, 180)) {
                glcd_puts("Erased   ", 10, 3, 0, 1, 0);
                database_erase();

                is_admin_mode = false;
                is_user_found = false;

                draw_main(is_user_found, user_name);
            } else if (IN_RANGE(x, 300, 480)) {
                glcd_puts("Scan card", 10, 3, 0, 1, 0);
                usart_buf_clear();

                is_adding = true;
            } else if (IN_RANGE(x, 580, 730)) {
                is_adding = false;
                is_admin_mode = false;
                is_user_found = false;

                draw_main(is_user_found, user_name);
            }
        }
    }
}
