#include <ch.h>
#include <hal.h>

int main(void)
{
    /* System initialization */
    halInit();
    chSysInit();

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
