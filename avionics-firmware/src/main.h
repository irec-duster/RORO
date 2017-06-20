#ifndef MAIN_H
#define MAIN_H

#include <ch.h>

/* Thread priorities */
#define GNSS_PRIO        NORMALPRIO+2
#define SERVO_PRIO       HIGHPRIO
#define PITOT_PRIO       NORMALPRIO
#define IMU_PRIO         NORMALPRIO+1
#define XBEE_PRIO        NORMALPRIO+1
#define HEARTBEAT_PRIO   LOWPRIO
#define SHELL_PRIO       LOWPRIO+1

extern BaseSequentialStream *debug;

#endif /* MAIN_H */
