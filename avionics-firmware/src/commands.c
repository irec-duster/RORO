#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <shell.h>
#include <string.h>

#include "gnss.h"
#include "imu.h"
#include "servo.h"
#include "main.h"
#include "msgbus/type_print.h"

static void cmd_bootloader(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)chp;
    (void)argc;
    (void)argv;
    reboot_st_bootloader();
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
    static const char *states[] = {CH_STATE_NAMES};
    thread_t *tp;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "    addr    stack prio refs     state       time\r\n");
    tp = chRegFirstThread();
    do {
        chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %10lu %s\r\n",
                (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                states[tp->p_state], (uint32_t)tp->p_time, tp->p_name);
        tp = chRegNextThread(tp);
    } while (tp != NULL);
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
        gnss_switch_upper_antenna();
    } else if (!strcmp(argv[0], "down")) {
        chprintf(chp, "select lower GNSS antenna.\n");
        gnss_switch_lower_antenna();
    }
}

static void cmd_gnss_forward(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)chp;
    (void)argc;
    (void)argv;
    while(1) {
        static uint8_t buf[100];
        size_t len = sdReadTimeout(&GNSS_SERIAL, buf, sizeof(buf), MS2ST(10));
        if (len > 0) {
            chSequentialStreamWrite(chp, buf, len);
        }
        if (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == 'q') {
            return;
        }
    }
}

void cmd_nosecone(BaseSequentialStream *chp, int argc, char *argv[])
{
    if (argc < 1) {
        chprintf(chp, "usage: nosecone lock|unlock\n");
    }

    if (!strcmp(argv[0], "lock")) {
        chprintf(chp, "Locking nosecone...\n");
        nosecone_locked = true;
    } else if (!strcmp(argv[0], "unlock")) {
        chprintf(chp, "Unlocking nosecone...\n");
        nosecone_locked = false;
    }
}

void cmd_glider(BaseSequentialStream *chp, int argc, char *argv[])
{
    if (argc < 1) {
        chprintf(chp, "usage: glider lock|unlock\n");
    }

    if (!strcmp(argv[0], "lock")) {
        chprintf(chp, "Locking glider...\n");
        glider_locked = true;
    } else if (!strcmp(argv[0], "unlock")) {
        chprintf(chp, "Unlocking glider...\n");
        glider_locked = false;
    }
}

static void cmd_topic_print(BaseSequentialStream *chp, int argc, char *argv[]) {
    if (argc < 1) {
        chprintf(chp, "usage: topic_print name (--watch|--stat)\n");
        return;
    }
    bool watch = false;
    bool stat = false;
    if (argc == 2 && !strcmp(argv[1], "--watch")) {
        watch = true;
    } else if (argc == 2 && !strcmp(argv[1], "--stat")) {
        stat = true;
    }
    msgbus_subscriber_t sub;
    if (msgbus_topic_subscribe(&sub, &bus, argv[0], MSGBUS_TIMEOUT_IMMEDIATE)) {
        if (msgbus_subscriber_topic_is_valid(&sub)) {
            msgbus_topic_t *topic = msgbus_subscriber_get_topic(&sub);
            const msgbus_type_definition_t *type = msgbus_topic_get_type(topic);

            char buf[type->struct_size];
            // void *buf = malloc(type->struct_size);
            // if (buf == NULL) {
            //     chprintf(chp, "malloc failed\n");
            //     return;
            // }
            msgbus_subscriber_read(&sub, buf);
            msgbus_print_type((void (*)(void *, const char *, ...))chprintf,
                              chp, type, buf);
            if (stat) {
                uint32_t count;
                count = msgbus_subscriber_has_update(&sub);
                chThdSleepSeconds(1);
                count = msgbus_subscriber_has_update(&sub) - count;
                chprintf(chp, "%s: %uHz\n", argv[0], count);
            }
            while (watch) {
                if (msgbus_subscriber_wait_for_update(&sub, 100000)) {
                    msgbus_subscriber_read(&sub, buf);
                    msgbus_print_type((void (*)(void *, const char *, ...))chprintf,
                                      chp, type, buf);
                }
                if (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == 'q') {
                    watch = false;
                }
            }
            // free(buf);
        } else {
            chprintf(chp, "topic not published yet\n");
            return;
        }
    } else {
        chprintf(chp, "topic doesn't exist\n");
        return;
    }
}

static void cmd_topic_list(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    msgbus_topic_t *topic = msgbus_iterate_topics(&bus);
    while (topic != NULL) {
        chprintf(chp, "%s\n", msgbus_topic_get_name(topic));
        topic = msgbus_iterate_topics_next(topic);
    }
}

static void cmd_simulate(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    chprintf(chp, "simulate launch...\n");
    chThdSleepSeconds(1);
    chprintf(chp, "launch\n");
    imu_raw_overwrite_data.acc[0] = 0.0;
    imu_raw_overwrite_data.acc[1] = 0.0;
    imu_raw_overwrite_data.acc[2] = 5.0;
    imu_overwrite = true;
    chThdSleepMilliseconds(150);
    imu_overwrite = false;

    chThdSleepSeconds(25);
    chprintf(chp, "apogee\n");

    chThdSleepSeconds(30);
    chprintf(chp, "main deployment\n");
    imu_raw_overwrite_data.acc[0] = -1.0;
    imu_raw_overwrite_data.acc[1] = 2.0;
    imu_raw_overwrite_data.acc[2] = -3.9;
    imu_overwrite = true;
    chThdSleepMilliseconds(100);
    imu_overwrite = false;
}

const ShellCommand shell_commands[] = {
    {"threads", cmd_threads},
    {"bootloader", cmd_bootloader},
    {"nosecone", cmd_nosecone},
    {"glider", cmd_glider},
    {"gnss_pwr", cmd_gnss_pwr},
    {"gnss_config", cmd_gnss_config},
    {"gnss_switch", cmd_gnss_switch},
    {"gnss_forward", cmd_gnss_forward},
    {"topics", cmd_topic_list},
    {"topic_print", cmd_topic_print},
    {"simulate", cmd_simulate},
    {NULL, NULL}
};
