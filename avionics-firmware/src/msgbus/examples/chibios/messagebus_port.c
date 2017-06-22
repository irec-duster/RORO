#include <ch.h>
#include "messagebus_port.h"
#include "../../messagebus.h"

void messagebus_lock_acquire(void *p)
{
    messagebus_condvar_wrapper_t *wrapper = (messagebus_condvar_wrapper_t *)p;
    chMtxLock(&wrapper->lock);
}

void messagebus_lock_release(void *p)
{
    messagebus_condvar_wrapper_t *wrapper = (messagebus_condvar_wrapper_t *)p;
    chMtxUnlock(&wrapper->lock);
}

void messagebus_condvar_broadcast(void *p)
{
    messagebus_condvar_wrapper_t *wrapper = (messagebus_condvar_wrapper_t *)p;
    chCondBroadcast(&wrapper->cond);
}

void messagebus_condvar_wait(void *p)
{
    messagebus_condvar_wrapper_t *wrapper = (messagebus_condvar_wrapper_t *)p;
    chCondWait(&wrapper->cond);
}

void messagebus_condvar_wrapper_init(messagebus_condvar_wrapper_t *c)
{
    chMtxObjectInit(&c->lock);
    chCondObjectInit(&c->cond);
}
