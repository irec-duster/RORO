#include <ch.h>
#include <hal.h>
#include "main.h"

// open 1.95ms
// closed 1.06ms
bool nosecone_locked = true;
bool glider_locked = true;

void servo5_timer_cb(GPTDriver *gptp)
{
    (void) gptp;
    palClearPad(GPIOE, GPIOE_SERVO_5);
}

void servo6_timer_cb(GPTDriver *gptp)
{
    (void) gptp;
    palClearPad(GPIOE, GPIOE_SERVO_6);
}

static THD_WORKING_AREA(servo_thread, 500);
void servo_thread_main(void *arg)
{
    (void) arg;
    chRegSetThreadName("servo");

    palSetPadMode(GPIOE, GPIOE_SERVO_5, PAL_MODE_OUTPUT_PUSHPULL);

    // nosecone servo timer
    static const GPTConfig tim6_config = {
        100000,
        servo5_timer_cb,
        0,  /* CR2 settings */
        0   /* DMA settings */
    };
    gptStart(&GPTD6, &tim6_config);

    // glider servo timer
    static const GPTConfig tim7_config = {
        100000,
        servo6_timer_cb,
        0,  /* CR2 settings */
        0   /* DMA settings */
    };
    gptStart(&GPTD7, &tim7_config);

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

        if (glider_locked) {
            chSysLock();
            palSetPad(GPIOE, GPIOE_SERVO_6);
            gptStartOneShotI(&GPTD7, 106); // TODO: set correct servo pulse in 0.01 ms
            chSysUnlock();
        } else {
            chSysLock();
            palSetPad(GPIOE, GPIOE_SERVO_6);
            gptStartOneShotI(&GPTD7, 195);
            chSysUnlock();
        }
        chThdSleepMilliseconds(20);
    }
}

void servo_init(void)
{
    chThdCreateStatic(&servo_thread, sizeof(servo_thread), SERVO_PRIO,
                      servo_thread_main, NULL);
}
