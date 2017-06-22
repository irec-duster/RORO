#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <shell.h>
#include <math.h>

#include "commands.h"
#include "usbcfg.h"
#include "servo.h"
#include "imu.h"
#include "gnss.h"
#include "xbee.h"
#include "pitot.h"
#include "types.h"
#include "main.h"

msgbus_t bus;

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


/* Note:
 * IMU raw acceleation vector:
 * 3rd elemant, positive for forward acceleration
*/

static THD_WORKING_AREA(deployment_thread, 2000);
void deployment_main(void *arg)
{
    (void)arg;
    chRegSetThreadName("deployment");

    imu_raw_t imu_raw;
    msgbus_subscriber_t imu_raw_sub;
    msgbus_topic_subscribe(&imu_raw_sub, &bus, "/imu/raw", MSGBUS_TIMEOUT_NEVER);

    nosecone_locked = true;
    glider_locked = false;

    // Glider mounting
    chThdSleepSeconds(5);
    glider_locked = true;
    chprintf(debug, "[%8u] glider lock\n", chVTGetSystemTime());

    // Wait for Launch
    bool hig_acc_detect;
    while (1) {
        msgbus_subscriber_read(&imu_raw_sub, &imu_raw);
        float acc_z = fabsf(imu_raw.acc[2]);

        systime_t hig_acc_start;
        if (acc_z > 4.0f) {
            if (hig_acc_detect == false) {
                hig_acc_start = chVTGetSystemTime();
            }
            hig_acc_detect = true;
        } else {
            hig_acc_detect = false;
        }

        // Launch detect
        systime_t now = chVTGetSystemTime();
        if (hig_acc_detect && ST2MS(now - hig_acc_start) > 100) {
            break;
        }

        chThdSleepMilliseconds(5);
    }
    chprintf(debug, "[%8u] launch detected\n", chVTGetSystemTime());

    // wait until apogee reached and drogue deployed
    chThdSleepSeconds(35); // time to apogee: 24.7s, delay: 10s
    chprintf(debug, "[%8u] deploy nosecone\n", chVTGetSystemTime());
    unsigned i = 10;
    while (i--) {   // 10 release cycles to be sure
        nosecone_locked = false;
        chThdSleepSeconds(1);
        nosecone_locked = true;
        chThdSleepSeconds(1);
    }
    nosecone_locked = false;

    // TODO: wait for main chute deployment at 400m
    // TODO: deploy glider after N seconds delay at approx 50m-100m AGL.

    while (1) {
        msgbus_subscriber_read(&imu_raw_sub, &imu_raw);
        float acc = sqrtf(imu_raw.acc[0]*imu_raw.acc[0]
                          + imu_raw.acc[1]*imu_raw.acc[1]
                          + imu_raw.acc[2]*imu_raw.acc[2]);

        systime_t hig_acc_start;
        if (acc > 4.0f) {
            if (hig_acc_detect == false) {
                hig_acc_start = chVTGetSystemTime();
            }
            hig_acc_detect = true;
        } else {
            hig_acc_detect = false;
        }

        // main chute deployment detect
        systime_t now = chVTGetSystemTime();
        if (hig_acc_detect && ST2MS(now - hig_acc_start) > 50) {
            break;
        }

        chThdSleepMilliseconds(5);
    }
    chThdSleepSeconds(20); // TODO
    chprintf(debug, "[%8u] deploy glider\n", chVTGetSystemTime());
    glider_locked = false;

    chThdExit(0);
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

    msgbus_init(&bus);

    shellInit();

    chprintf(debug, "start XBEE...\n");
    xbee_start();

    chprintf(debug, "start GPS...\n");
    gnss_start();

    chprintf(debug, "start IMU...\n");
    imu_start();

    chprintf(debug, "start Pitot...\n");
    pitot_start();

    chprintf(debug, "start deployment thread\n");
    chThdCreateStatic(&deployment_thread, sizeof(deployment_thread), DEPLOYMENT_PRIO,
                      deployment_main, NULL);

    usb_init();
    while (true) {
        spawn_shell(usb);
        chThdSleepMilliseconds(100);
    }
}
