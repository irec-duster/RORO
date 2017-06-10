#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <shell.h>

static void cmd_bootloader(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)chp;
    (void)argc;
    (void)argv;
    reboot_st_bootloader();
}

const ShellCommand shell_commands[] = {
    {"bootloader", cmd_bootloader},
    {NULL, NULL}
};
