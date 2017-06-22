#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#include "main.h"

BaseSequentialStream *xbee = NULL;
static void xbee_uart_init(void)
{
    static const SerialConfig xbee_serial_conf = {
        57600,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0 // USART_CR3_RTSE | USART_CR3_CTSE /* enable RTS/CTS flow control */
    };
    sdStart(&SD3, &xbee_serial_conf);
    xbee = (BaseSequentialStream *)&SD3;
}

static THD_WORKING_AREA(xbee_thread, 2000);
void xbee_main(void *arg)
{
    (void)arg;
    chRegSetThreadName("Xbee");

    while (true) {

        chThdSleepMilliseconds(100);
    }
}

void xbee_start(void)
{
    xbee_uart_init();

    chThdCreateStatic(&xbee_thread, sizeof(xbee_thread), XBEE_PRIO, xbee_main, NULL);
}
