#ifndef GNSS_H
#define GNSS_H

#include <ch.h>
#include <hal.h>

extern BaseSequentialStream *gnss;
#define GNSS_SERIAL SD6

#define GNSS_NMEA_MAX_SENTENCE 200

extern bool nmea_gngga_update;
extern char nmea_gngga_sentence[GNSS_NMEA_MAX_SENTENCE];
extern bool nmea_gngll_update;
extern char nmea_gngll_sentence[GNSS_NMEA_MAX_SENTENCE];

void gnss_switch_upper_antenna(void);
void gnss_switch_lower_antenna(void);

void gnss_start(void);

#endif /* GNSS_H */
