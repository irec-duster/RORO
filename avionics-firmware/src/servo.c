#include <ch.h>
#include <hal.h>

// open 1.95ms
// closed 1.06ms
bool nosecone_locked = true;

void servo_timer_cb(GPTDriver *gptp)
{
    (void) gptp;
    palClearPad(GPIOE, GPIOE_SERVO_5);
}

static THD_WORKING_AREA(servo_thread, 500);
void servo_thread_main(void *arg)
{
    (void) arg;

    palSetPadMode(GPIOE, GPIOE_SERVO_5, PAL_MODE_OUTPUT_PUSHPULL);

    static const GPTConfig tim6_config = {
        100000,
        servo_timer_cb,
        0,  /* CR2 settings */
        0   /* DMA settings */
    };
    gptStart(&GPTD6, &tim6_config);

    while (1) {
        if (nosecone_locked) {
            chSysLock();
            palSetPad(GPIOE, GPIOE_SERVO_5);
            gptStartOneShotI(&GPTD6, 106);
            chSysUnlock();
        } else {
            chSysLock();
            palSetPad(GPIOE, GPIOE_SERVO_5);
            gptStartOneShotI(&GPTD6, 195);
            chSysUnlock();
        }
        chThdSleepMilliseconds(20);
    }
}

void servo_init(void)
{
    chThdCreateStatic(&servo_thread, sizeof(servo_thread), HIGHPRIO,
                      servo_thread_main, NULL);
}
