#include "../msgbus.h"
#include "mocks/synchronization.hpp"
#include "types/test.h"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


TEST_GROUP(MessageBusAtomicityTestGroup)
{
    msgbus_t bus;
    msgbus_topic_t topic;
    simple_t buffer;

    void setup()
    {
        mock().strictOrder();

        msgbus_init(&bus);
        msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");
    }

    void teardown()
    {
        lock_mocks_enable(false);
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(MessageBusAtomicityTestGroup, AdvertiseIsLocked)
{
    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &bus.lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &bus.lock);

    lock_mocks_enable(true);
    msgbus_init(&bus);
    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");
}

TEST(MessageBusAtomicityTestGroup, FindNoneIsLocked)
{
    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &bus.lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &bus.lock);

    lock_mocks_enable(true);
    msgbus_find_topic(&bus, "notatopic", MSGBUS_TIMEOUT_IMMEDIATE);
}

TEST(MessageBusAtomicityTestGroup, FindExistingTopicIsLocked)
{
    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &bus.lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &bus.lock);

    lock_mocks_enable(true);
    msgbus_find_topic(&bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
}

TEST(MessageBusAtomicityTestGroup, FindBlockingIsLocked)
{
    lock_mocks_enable(true);

    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &bus.lock);

    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &bus.lock);

    msgbus_find_topic(&bus, "topic", MSGBUS_TIMEOUT_NEVER);
}

TEST(MessageBusAtomicityTestGroup, IterateIsLocked)
{
    lock_mocks_enable(true);

    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &bus.lock);

    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &bus.lock);

    msgbus_iterate_topics(&bus);
}

TEST(MessageBusAtomicityTestGroup, IterateNextIsNotLocked)
{
    msgbus_topic_t *i = msgbus_iterate_topics(&bus);
    lock_mocks_enable(true);
    // there is no lock for next, because next pointer is constant
    msgbus_iterate_topics_next(i);
}

TEST(MessageBusAtomicityTestGroup, PublishIsLocked)
{
    simple_t var;
    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &topic.bus->topic_update_lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &topic.bus->topic_update_lock);

    lock_mocks_enable(true);
    msgbus_topic_publish(&topic, &var);
}

TEST(MessageBusAtomicityTestGroup, SubscriberWaitIsLocked)
{
    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &topic.bus->topic_update_lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &topic.bus->topic_update_lock);

    msgbus_subscriber_t sub;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    lock_mocks_enable(true);
    msgbus_subscriber_wait_for_update(&sub, MSGBUS_TIMEOUT_IMMEDIATE);
}


TEST(MessageBusAtomicityTestGroup, HasUpdateIsLocked)
{
    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &topic.bus->topic_update_lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &topic.bus->topic_update_lock);

    msgbus_subscriber_t sub;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    lock_mocks_enable(true);
    msgbus_subscriber_has_update(&sub);
}

TEST(MessageBusAtomicityTestGroup, IsValidIsLocked)
{
    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &topic.bus->topic_update_lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &topic.bus->topic_update_lock);

    msgbus_subscriber_t sub;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    lock_mocks_enable(true);
    msgbus_subscriber_topic_is_valid(&sub);
}

TEST(MessageBusAtomicityTestGroup, ReadIsLocked)
{
    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &topic.bus->topic_update_lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &topic.bus->topic_update_lock);

    msgbus_subscriber_t sub;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    lock_mocks_enable(true);
    simple_t var;
    msgbus_subscriber_read(&sub, &var);
}
