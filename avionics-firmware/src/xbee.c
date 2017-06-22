#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <string.h>

#include "gnss.h"
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

    // msgbus_subscriber_t imu_raw_sub;
    // msgbus_topic_subscribe(&imu_raw_sub, &bus, "/imu/raw", MSGBUS_TIMEOUT_NEVER);

    // msgbus_subscriber_t imu_quaternion_sub;
    // msgbus_topic_subscribe(&imu_quaternion_sub, &bus, "/imu/quaternion", MSGBUS_TIMEOUT_NEVER);
    while (true) {
        static char gnss_buf[GNSS_NMEA_MAX_SENTENCE];
        if (nmea_gngga_update) {
            chSysLock();
            strcpy(gnss_buf, nmea_gngll_sentence);
            nmea_gngll_update = false;
            chSysUnlock();
            chprintf(xbee, "%s\n", gnss_buf);
        }
        if (nmea_gngga_update) {
            chSysLock();
            strcpy(gnss_buf, nmea_gngga_sentence);
            nmea_gngga_update = false;
            chSysUnlock();
            chprintf(xbee, "%s\n", gnss_buf);
        }
        chThdSleepMilliseconds(100);
    }
}

void xbee_start(void)
{
    xbee_uart_init();

    chThdCreateStatic(&xbee_thread, sizeof(xbee_thread), XBEE_PRIO, xbee_main, NULL);
}
