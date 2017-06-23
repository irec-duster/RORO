#ifndef MAIN_H
#define MAIN_H

#include <ch.h>
#include <msgbus/msgbus.h>

/* Thread priorities */
#define DEPLOYMENT_PRIO  HIGHPRIO-1
#define GNSS_PRIO        NORMALPRIO+2
#define SERVO_PRIO       HIGHPRIO
#define PITOT_PRIO       NORMALPRIO
#define IMU_PRIO         NORMALPRIO+1
#define XBEE_PRIO        NORMALPRIO+1
#define BUZZER_PRIO      LOWPRIO+2
#define HEARTBEAT_PRIO   LOWPRIO
#define SHELL_PRIO       LOWPRIO+1

extern BaseSequentialStream *debug;
extern msgbus_t bus;

#endif /* MAIN_H */
