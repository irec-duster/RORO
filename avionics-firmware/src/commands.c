#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <shell.h>
#include <string.h>

#include "main.h"

static void cmd_bootloader(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)chp;
    (void)argc;
    (void)argv;
    reboot_st_bootloader();
}

static void cmd_gnss_pwr(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    if (argc < 1) {
        chprintf(chp, "usage: gnss_pwr on|off\n");
    }

    if (!strcmp(argv[0], "on")) {
        chprintf(chp, "GNSS on.\n");
        palClearPad(GPIOC, GPIOC_GPS_EN_N);
    } else if (!strcmp(argv[0], "off")) {
        chprintf(chp, "GNSS off.\n");
        palSetPad(GPIOC, GPIOC_GPS_EN_N);
    }
}

static void cmd_gnss_config(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    chprintf(chp, "Configure GNSS UART pin as input... ");
    palSetPadMode(GPIOC, GPIOC_UART6_TX, PAL_MODE_INPUT);
    // palSetPadMode(GPIOC, GPIOC_UART6_RX, PAL_MODE_INPUT);
    chprintf(chp, "Done\n");
    chprintf(chp, "You can now configure the GNSS.\n");
}

static void cmd_gnss_switch(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)chp;
    (void)argc;
    (void)argv;
    if (argc < 1) {
        chprintf(chp, "usage: gnss_switch up|down\n");
    }

    if (!strcmp(argv[0], "up")) {
        chprintf(chp, "select upper GNSS antenna.\n");
        palSetPad(GPIOC, GPIOC_GPS_SWITCH);
    } else if (!strcmp(argv[0], "down")) {
        chprintf(chp, "select lower GNSS antenna.\n");
        palClearPad(GPIOC, GPIOC_GPS_SWITCH);
    }
}

static void cmd_gnss_forward(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)chp;
    (void)argc;
    (void)argv;
    // // todo
    // palSetPadMode(GPIOC, GPIOC_UART6_TX, PAL_MODE_ALTERNATE(8));
    // palSetPadMode(GPIOC, GPIOC_UART6_RX, PAL_MODE_ALTERNATE(8) | PAL_STM32_PUPDR_PULLUP);

    while(1) {
        static uint8_t buf[100];
        size_t len = sdReadTimeout(&SD6, buf, sizeof(buf), MS2ST(10));
        if (len > 0) {
            chSequentialStreamWrite(chp, buf, len);
        }
        if (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == 'q') {
            return;
        }
    }
}

const ShellCommand shell_commands[] = {
    {"bootloader", cmd_bootloader},
    {"gnss_pwr", cmd_gnss_pwr},
    {"gnss_config", cmd_gnss_config},
    {"gnss_switch", cmd_gnss_switch},
    {"gnss_forward", cmd_gnss_forward},
    {NULL, NULL}
};
