#ifndef MESSAGEBUS_PORT_H
#define MESSAGEBUS_PORT_H

#include <ch.h>

typedef struct {
    mutex_t lock;
    condition_variable_t cond;
} messagebus_condvar_wrapper_t;


#ifdef __cplusplus
extern "C" {
#endif

void messagebus_condvar_wrapper_init(messagebus_condvar_wrapper_t *c);

#ifdef __cplusplus
}
#endif


#endif /* MESSAGEBUS_PORT_H */
