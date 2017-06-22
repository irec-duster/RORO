#ifndef MSGBUS_PORT_H
#define MSGBUS_PORT_H

#include <pthread.h>
#include <assert.h>

typedef pthread_cond_t msgbus_cond_t;
typedef pthread_mutex_t msgbus_mutex_t;


#define MSGBUS_ASSERT(x) assert(x)

#endif /* MSGBUS_PORT_H */
