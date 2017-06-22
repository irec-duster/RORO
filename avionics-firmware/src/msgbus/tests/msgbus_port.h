#ifndef MSGBUS_PORT_H
#define MSGBUS_PORT_H

// port file for the unit tests

typedef int msgbus_cond_t;
typedef int msgbus_mutex_t;

void msgbus_unittest_assert(bool condition);
#define MSGBUS_ASSERT(x) msgbus_unittest_assert(x)

#endif /* MSGBUS_PORT_H */
