#ifndef MSGBUS_PORT_H
#define MSGBUS_PORT_H

#include <ch.h>
#include <osal.h>

typedef condition_variable_t msgbus_cond_t;
typedef mutex_t msgbus_mutex_t;

#define MSGBUS_ASSERT(x) osalDbgAssert(x, "msgbus assert")

#endif /* MSGBUS_PORT_H */
