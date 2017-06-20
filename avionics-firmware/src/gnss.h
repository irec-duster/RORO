#ifndef GNSS_H
#define GNSS_H

#include <ch.h>
#include <hal.h>

extern BaseSequentialStream *gnss;
#define GNSS_SERIAL SD6

void gnss_switch_upper_antenna(void);
void gnss_switch_lower_antenna(void);

void gnss_start(void);

#endif /* GNSS_H */
