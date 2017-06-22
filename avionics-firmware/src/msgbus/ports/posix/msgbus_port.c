#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include "msgbus_port.h"
#include "../../msgbus.h"


void msgbus_mutex_init(msgbus_mutex_t *mutex)
{
    assert(pthread_mutex_init(mutex, NULL) == 0);
}

void msgbus_mutex_acquire(msgbus_mutex_t *mutex)
{
    assert(pthread_mutex_lock(mutex) == 0);
}

void msgbus_mutex_release(msgbus_mutex_t *mutex)
{
    assert(pthread_mutex_unlock(mutex) == 0);
}

void msgbus_condvar_init(msgbus_cond_t *cond)
{
    assert(pthread_cond_init(cond, NULL) == 0);
}

void msgbus_condvar_broadcast(msgbus_cond_t *cond)
{
    assert(pthread_cond_broadcast(cond) == 0);
}

#ifdef __APPLE__ // OS X has no gettitme with realtime clock
bool msgbus_condvar_wait(msgbus_cond_t *cond, msgbus_mutex_t *mutex, uint32_t timeout_us)
{
    if (timeout_us == MSGBUS_TIMEOUT_IMMEDIATE) {
        return false;
    }
    if (timeout_us == MSGBUS_TIMEOUT_NEVER) {
        assert(pthread_cond_wait(cond, mutex) == 0);
    } else {
        struct timespec ts;
        ts.tv_sec = timeout_us / 1000000;
        ts.tv_nsec = (timeout_us % 1000000) * 1000;
        int ret = pthread_cond_timedwait_relative_np(cond, mutex, &ts);
        assert(ret == 0 || ret == ETIMEDOUT);
        if (ret == ETIMEDOUT) {
            return false;
        }
    }
    return true;
}
#else // posix
bool msgbus_condvar_wait(msgbus_cond_t *cond, msgbus_mutex_t *mutex, uint32_t timeout_us)
{
    if (timeout_us == MSGBUS_TIMEOUT_IMMEDIATE) {
        return false;
    }
    if (timeout_us == MSGBUS_TIMEOUT_NEVER) {
        assert(pthread_cond_wait(cond, mutex) == 0);
    } else {
        struct timespec ts;
        assert(clock_gettime(CLOCK_REALTIME, &ts) == 0);
        ts.tv_nsec += (timeout_us % 1000000) * 1000;
        ts.tv_sec += timeout_us / 1000000 + ts.tv_nsec / 1000000000;
        ts.tv_nsec = ts.tv_nsec % 1000000000;
        int ret = pthread_cond_timedwait(cond, mutex, &ts);
        assert(ret == 0 || ret == ETIMEDOUT);
        if (ret == ETIMEDOUT) {
            return false;
        }
    }
    return true;
}
#endif
