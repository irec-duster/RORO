#include <ch.h>
#include <hal.h>
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

static THD_WORKING_AREA(gnss_thread, 2000);
void gnss_main(void *arg)
{
    (void)arg;

    while (1) {
        chThdSleepMilliseconds(100);
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
