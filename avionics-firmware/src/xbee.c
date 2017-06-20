#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <shell.h>
#include <string.h>

#include "main.h"

static void cmd_start(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // XXX TODO: start sequence
    chprintf(chp, "do stuff\n");
    chThdSleepMilliseconds(100);

    chprintf(chp, "exit shell\n");
    shellExit(0);
}

static void cmd_reset(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // XXX TODO: implement proper way to shut down

    chprintf(chp, "shutting down GNSS\n");
    palSetPad(GPIOC, GPIOC_GPS_EN_N);

    chprintf(chp, "shutting down IMU\n");
    palSetPad(GPIOD, GPIOD_IMU_EN_N);

    chprintf(chp, "shutting down pitot\n");
    palSetPad(GPIOB, GPIOB_DIF_P_SENS_EN_N);

    chprintf(chp, "shutting down barometer\n");
    palSetPad(GPIOB, GPIOB_ABS_P_SENS_EN_N);

    chThdSleepMilliseconds(100);
    NVIC_SystemReset();
}

static const ShellCommand xbee_commands[] = {
    {"reset", cmd_reset},
    {"START", cmd_start},
    {NULL, NULL}
};


static void spawn_shell(BaseSequentialStream *shell_dev)
{
    static thread_t *shelltp = NULL;
    static ShellConfig shell_cfg;
    static THD_WORKING_AREA(shell_wa, 4000);

    shell_cfg.sc_channel = shell_dev;
    shell_cfg.sc_commands = xbee_commands;

    if (shelltp == NULL && shell_dev != NULL) {
        shelltp = shellCreateStatic(&shell_cfg, shell_wa, sizeof(shell_wa), LOWPRIO);
    } else if (shelltp != NULL && chThdTerminatedX(shelltp)) {
        shelltp = NULL;
    }
}

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

    while (true) {
        spawn_shell(xbee);
        chThdSleepMilliseconds(100);
    }
}

void xbee_start(void)
{
    xbee_uart_init();

    chThdCreateStatic(&xbee_thread, sizeof(xbee_thread), XBEE_PRIO, xbee_main, NULL);
}
