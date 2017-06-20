#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <shell.h>

#include "commands.h"
#include "usbcfg.h"
#include "servo.h"
#include "imu.h"
#include "gnss.h"
#include "xbee.h"
#include "pitot.h"
#include "main.h"

static THD_WORKING_AREA(heartbeat_thread, 200);
void heartbeat_main(void *arg)
{
    (void) arg;

    chRegSetThreadName("heartbeat");

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
        shelltp = shellCreateStatic(&shell_cfg, shell_wa, sizeof(shell_wa), SHELL_PRIO);
    } else if (shelltp != NULL && chThdTerminatedX(shelltp)) {
        shelltp = NULL;
    }
}

void panic(const char *reason)
{
    (void) reason;

    // shutting down GNSS
    palSetPad(GPIOC, GPIOC_GPS_EN_N);

    // shutting down IMU
    palSetPad(GPIOD, GPIOD_IMU_EN_N);

    // shutting down pitot
    palSetPad(GPIOB, GPIOB_DIF_P_SENS_EN_N);

    // shutting down barometer
    palSetPad(GPIOB, GPIOB_ABS_P_SENS_EN_N);

    // wait a bit to properly reset peripherals
    unsigned i = 1000000;
    while (i--) {
        __asm__ volatile ("nop": : :);
    }

    NVIC_SystemReset();
}

int main(void)
{
    /* System initialization */
    halInit();
    chSysInit();

    /* Heartbeat thread */
    chThdCreateStatic(&heartbeat_thread, sizeof(heartbeat_thread), HEARTBEAT_PRIO,
                      heartbeat_main, NULL);

    servo_init();

    debug_uart_init();
    chprintf(debug, "boot\n");

    shellInit();

    chprintf(debug, "start XBEE shell...\n");
    xbee_start();

    chprintf(debug, "start GPS...\n");
    gnss_start();

    chprintf(debug, "start IMU...\n");
    imu_start();

    chprintf(debug, "start Pitot...\n");
    pitot_start();

    usb_init();
    while (true) {
        spawn_shell(usb);
        chThdSleepMilliseconds(100);
    }
}
