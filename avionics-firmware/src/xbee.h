#ifndef XBEE_H
#define XBEE_H

#include <ch.h>

extern BaseSequentialStream *xbee;

void xbee_start(void);

#endif /* XBEE_H */
