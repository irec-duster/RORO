#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <shell.h>

#include "commands.h"
#include "usbcfg.h"
#include "servo.h"
#include "main.h"

static THD_WORKING_AREA(heartbeat_thread, 200);
void heartbeat_main(void *arg)
{
    (void) arg;
    while (true) {
        palSetPad(GPIOC, GPIOC_LED2);
        chThdSleepMilliseconds(80);
        palClearPad(GPIOC, GPIOC_LED2);
        chThdSleepMilliseconds(80);
        palSetPad(GPIOC, GPIOC_LED2);
        chThdSleepMilliseconds(80);
        palClearPad(GPIOC, GPIOC_LED2);
        chThdSleepMilliseconds(760);
    }
}

BaseSequentialStream *xbee = NULL;
static void xbee_uart_init(void)
{
    /* XBee UART init */
    static const SerialConfig xbee_serial_conf = {
        57600,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
    };
    sdStart(&SD3, &xbee_serial_conf);
    xbee = (BaseSequentialStream *)&SD3;
}

BaseSequentialStream *debug = NULL;
static void debug_uart_init(void)
{
    /* Front panel debug UART init */
    static const SerialConfig debug_serial_conf = {
        57600,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
    };
    sdStart(&SD4, &debug_serial_conf);
    debug = (BaseSequentialStream *)&SD4;
}

BaseSequentialStream *gnss = NULL;
static void gnss_uart_init(void)
{
    /* Front panel gnss UART init */
    static const SerialConfig gnss_serial_conf = {
        19200,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
    };
    sdStart(&SD6, &gnss_serial_conf);
    gnss = (BaseSequentialStream *)&SD4;
}

BaseSequentialStream *usb = NULL;
static void usb_init(void)
{
    /* USB serial driver */
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    usb = (BaseSequentialStream *)&SDU1;
}

static void spawn_shell(BaseSequentialStream *shell_dev)
{
    static thread_t *shelltp = NULL;
    static ShellConfig shell_cfg;
    static THD_WORKING_AREA(shell_wa, 2048);

    shell_cfg.sc_channel = shell_dev;
    shell_cfg.sc_commands = shell_commands;

    if (shelltp == NULL && shell_dev != NULL) {
        shelltp = shellCreateStatic(&shell_cfg, shell_wa, sizeof(shell_wa), LOWPRIO);
    } else if (shelltp != NULL && chThdTerminatedX(shelltp)) {
        shelltp = NULL;
    }
}

int main(void)
{
    /* System initialization */
    halInit();
    chSysInit();

    /* Heartbeat thread */
    chThdCreateStatic(&heartbeat_thread, sizeof(heartbeat_thread), NORMALPRIO,
                      heartbeat_main, NULL);

    servo_init();

    debug_uart_init();
    chprintf(debug, "boot\n");

    xbee_uart_init();
    gnss_uart_init();
    chprintf(debug, "enable GPS...\n");
    palClearPad(GPIOC, GPIOC_GPS_EN_N);

    palSetPadMode(GPIOC, GPIOC_UART6_TX, PAL_MODE_INPUT);
    usb_init();


    shellInit();
    while (true) {
        spawn_shell(usb);
        chThdSleepMilliseconds(100);
    }
}
