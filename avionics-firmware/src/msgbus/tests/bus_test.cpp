#include "../msgbus.h"
#include "mocks/synchronization.hpp"
#include "types/test.h"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


TEST_GROUP(BusTests)
{
    msgbus_t bus;
    msgbus_topic_t topic;
    simple_t buffer;
    msgbus_topic_t second_topic;

    void setup()
    {
        msgbus_init(&bus);
    }

    void teardown()
    {
        condvar_init_mock_enable(false);
        mock().checkExpectations();
        mock().clear();
    }
};


TEST(BusTests, Initializer)
{
    mock().expectOneCall("msgbus_condvar_init").withParameter("cond", &bus.condvar);
    mock().expectOneCall("msgbus_mutex_init").withParameter("mutex", &bus.lock);
    mock().expectOneCall("msgbus_mutex_init").withParameter("mutex", &bus.topic_update_lock);
    condvar_init_mock_enable(true);
    memset(&bus, 0xff, sizeof(bus));
    msgbus_init(&bus);
    POINTERS_EQUAL(NULL, bus.topics.head);
}


TEST(BusTests, FirstTopicGoesToHead)
{
    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");

    POINTERS_EQUAL(&topic, bus.topics.head);
}

TEST(BusTests, NextofListIsOkToo)
{
    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "first");
    msgbus_topic_create(&second_topic, &bus, &simple_type, &buffer, "second");

    POINTERS_EQUAL(&second_topic, bus.topics.head);
    POINTERS_EQUAL(&topic, bus.topics.head->next);
}

TEST(BusTests, FindTopicNotFound)
{
    POINTERS_EQUAL(NULL, msgbus_find_topic(&bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE));
}

TEST(BusTests, FindTopicFound)
{
    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");
    POINTERS_EQUAL(&topic, msgbus_find_topic(&bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE));
}

TEST(BusTests, FindOnBusWithMultipleTopics)
{
    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "first");
    msgbus_topic_create(&second_topic, &bus, &simple_type, &buffer, "second");

    POINTERS_EQUAL(&topic, msgbus_find_topic(&bus, "first", MSGBUS_TIMEOUT_IMMEDIATE));
    POINTERS_EQUAL(&second_topic, msgbus_find_topic(&bus, "second", MSGBUS_TIMEOUT_IMMEDIATE));
}

TEST(BusTests, FindTopicBlocking)
{
    msgbus_topic_t *res;
    /* This is a partial test only: we cannot test that the behavior is correct
     * when the topic is not on the bus yes without additional thread and I
     * don't like threading in tests. */
    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");
    res = msgbus_find_topic(&bus, "topic", MSGBUS_TIMEOUT_NEVER);
    POINTERS_EQUAL(&topic, res);
}

TEST(BusTests, IterateTopics)
{
    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "first");
    msgbus_topic_create(&second_topic, &bus, &simple_type, &buffer, "second");

    msgbus_topic_t *i = msgbus_iterate_topics(&bus);
    POINTERS_EQUAL(&second_topic, i);
    i = msgbus_iterate_topics_next(i);
    POINTERS_EQUAL(&topic, i);
    i = msgbus_iterate_topics_next(i);
    POINTERS_EQUAL(NULL, i);
}
