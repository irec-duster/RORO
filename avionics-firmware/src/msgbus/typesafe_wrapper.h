#include "msgbus.h"
#include "type_definition.h"

#define MsgBusSubscriber(typename) msgbus::Subscriber<typename##_t, &typename##_type>
#define MsgBusTopic(typename) msgbus::Topic<typename##_t, &typename##_type>


namespace msgbus {


template <typename msg_type, const msgbus_type_definition_t *msg_type_def>
class Topic
{
    msg_type buffer;
    bool is_advertised = false;
public:
    msgbus_topic_t topic;

    void advertise(msgbus_t *bus, const char *name)
    {
        msgbus_topic_create(&topic, bus, msg_type_def, &buffer, name);
        is_advertised = true;
    }

    void publish(const msg_type &val)
    {
        MSGBUS_ASSERT(is_advertised);
        msgbus_topic_publish(&topic, &val);
    }
};


template <typename msg_type, const msgbus_type_definition_t *msg_type_def>
class Subscriber
{
public:
    msgbus_subscriber_t sub;

    bool subscribe(msgbus_t *bus, const char *name)
    {
        return subscribe(bus, name, MSGBUS_TIMEOUT_NEVER);
    }

    bool subscribe(msgbus_t *bus, const char *name, uint64_t timeout_us)
    {
        bool valid = msgbus_topic_subscribe(&sub, bus, name, timeout_us);
        if (valid) {
            msgbus_topic_t *topic = msgbus_subscriber_get_topic(&sub);
            MSGBUS_ASSERT(msg_type_def == msgbus_topic_get_type(topic)); // type checking
            return true;
        } else {
            return false;
        }
    }

    bool is_valid()
    {
        return msgbus_subscriber_topic_is_valid(&sub);
    }

    uint32_t has_update()
    {
        return msgbus_subscriber_has_update(&sub);
    }

    uint32_t read(msg_type &val)
    {
        return msgbus_subscriber_read(&sub, &val);
    }

    bool wait_for_update()
    {
        return msgbus_subscriber_wait_for_update(&sub, MSGBUS_TIMEOUT_NEVER);
    }

    bool wait_for_update(uint64_t timeout_us)
    {
        return msgbus_subscriber_wait_for_update(&sub, timeout_us);
    }
};


} // namespace msgbus
