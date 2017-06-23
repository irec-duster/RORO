#include "msgbus_port.h"
#include "../../msgbus.h"

void msgbus_mutex_init(msgbus_mutex_t *mutex)
{
    chMtxObjectInit(mutex);
}

void msgbus_mutex_acquire(msgbus_mutex_t *mutex)
{
    chMtxLock(mutex);
}

void msgbus_mutex_release(msgbus_mutex_t *mutex)
{
    chMtxUnlock(mutex);
}

void msgbus_condvar_init(msgbus_cond_t *cond)
{
    chCondObjectInit(cond);
}

void msgbus_condvar_broadcast(msgbus_cond_t *cond)
{
    chCondBroadcast(cond);
}

bool msgbus_condvar_wait(msgbus_cond_t *cond, msgbus_mutex_t *mutex, uint32_t timeout_us)
{
    (void)mutex;

    if (timeout_us == MSGBUS_TIMEOUT_IMMEDIATE) {
        return false;
    }
    systime_t timeout;
    if (timeout_us == MSGBUS_TIMEOUT_NEVER) {
        timeout = TIME_INFINITE;
    } else {
        timeout = US2ST(timeout_us);
    }
    msg_t ret = chCondWaitTimeout(cond, timeout);
    if (ret == MSG_TIMEOUT) {
        chMtxLock(mutex);
        return false;
    } else {
        return true;
    }
}
