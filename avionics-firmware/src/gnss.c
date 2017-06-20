#include <ch.h>
#include <hal.h>
#include <string.h>

#include "main.h"
#include "gnss.h"

/*
   Enable:
   GPIO: GPIOC_GPS_EN_N

   Antenna switch:
   GPIO: GPIOC_GPS_SWITCH
   CTRL = HIGH: RF1, P33, up
   CTRL = LOW: RF2, P34, down
 */

/*
   baudrate: 19200

   Messages:

   UBX NAV SOL
   UBX NAV CLK
   UBX RXM RAWX

 */

BaseSequentialStream *gnss = NULL;
static void gnss_uart_init(void)
{
    static const SerialConfig gnss_serial_conf = {
        19200,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
    };
    sdStart(&GNSS_SERIAL, &gnss_serial_conf);
    gnss = (BaseSequentialStream *)&GNSS_SERIAL;
}

bool buffer_push_byte(char **wp, const char *end, char c)
{
    if (*wp < end) {
        *(*wp)++ = c;
    } else {
        return true;
    }
    return false;
}

bool nmea_gngga_update = false;
char gngga_sentence[GNSS_NMEA_MAX_SENTENCE];
bool nmea_gngll_update = false;
char gngll_sentence[GNSS_NMEA_MAX_SENTENCE];

static THD_WORKING_AREA(gnss_thread, 2000);
void gnss_main(void *arg)
{
    (void)arg;

    static char buf[GNSS_NMEA_MAX_SENTENCE];
    const char *buf_end = &buf[0] + sizeof(buf) - 1;
    char *wp = &buf[0];

    bool nmea_sentence_found = false;
    while (1) {
        char c = sdGet(&GNSS_SERIAL);
        if (c == '$') {
            nmea_sentence_found = true;
            wp = &buf[0];
        }
        if (nmea_sentence_found) {
            if (buffer_push_byte(&wp, buf_end, c)) {
                // overflow
                nmea_sentence_found = false;
                continue;
            }

            // end of sentence
            if (c == '\n' || c == '\r') {

                *wp = 0; // 0-terminate

                if (strcmp(buf, "$GNGGA")) {
                    strcpy(gngga_sentence, buf);
                    nmea_gngga_update = true;
                } else if (strcmp(buf, "$GNGLL")) {
                    strcpy(gngll_sentence, buf);
                    nmea_gngll_update = true;
                }
                // XXX TODO: add more NMEA messages here.

                nmea_sentence_found = false;
            }
        }
    }
}

void gnss_start(void)
{
    gnss_uart_init();

    /* GNSS enable */
    palClearPad(GPIOC, GPIOC_GPS_EN_N);

    gnss_switch_upper_antenna();

    chThdCreateStatic(&gnss_thread, sizeof(gnss_thread), GNSS_PRIO, gnss_main, NULL);
}

void gnss_switch_upper_antenna(void)
{
    // select upper antenna
    palSetPad(GPIOC, GPIOC_GPS_SWITCH);
}

void gnss_switch_lower_antenna(void)
{
    // select lower antenna
    palClearPad(GPIOC, GPIOC_GPS_SWITCH);
}
