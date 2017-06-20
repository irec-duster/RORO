#ifndef MAIN_H
#define MAIN_H

#include <ch.h>

#define GNSS_PRIO        NORMALPRIO+1
#define SERVO_PRIO       HIGHPRIO
#define PITOT_PRIO       NORMALPRIO
#define HEARTBEAT_PRIO   LOWPRIO
#define SHELL_PRIO       LOWPRIO+1

extern BaseSequentialStream *xbee;
extern BaseSequentialStream *debug;

#endif /* MAIN_H */
