#include "../msgbus.h"
#include "mocks/synchronization.hpp"
#include "types/test.h"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


TEST_GROUP(SubscriberTests)
{
    msgbus_t bus;
    msgbus_topic_t topic;
    simple_t buffer;
    msgbus_subscriber_t sub;

    void setup()
    {
        msgbus_init(&bus);
        msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");
    }

    void teardown()
    {
        condvar_init_mock_enable(false);
        mock().checkExpectations();
        mock().clear();
    }
};


TEST(SubscriberTests, Subscribe)
{
    bool ret;
    ret = msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    CHECK_TRUE(ret);
    CHECK_EQUAL(sub.topic, &topic);
}

TEST(SubscriberTests, SubscribeFailNonExisting)
{
    bool ret;
    ret = msgbus_topic_subscribe(&sub, &bus, "notatopic", MSGBUS_TIMEOUT_IMMEDIATE);
    CHECK_FALSE(ret);
}

TEST(SubscriberTests, SubscriberIsInvalidWhenUnpublished)
{
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    CHECK_FALSE(msgbus_subscriber_topic_is_valid(&sub));
}

TEST(SubscriberTests, SubscriberIsValidAfterPublish)
{
    simple_t val;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    msgbus_topic_publish(&topic, &val);
    CHECK_TRUE(msgbus_subscriber_topic_is_valid(&sub));
}

TEST(SubscriberTests, SubscriberRemainsValidAfterRead)
{
    simple_t val;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    msgbus_topic_publish(&topic, &val);
    msgbus_subscriber_read(&sub, &val); // even if topic is read
    CHECK_TRUE(msgbus_subscriber_topic_is_valid(&sub));
}

TEST(SubscriberTests, SubscriberHasNoUpdateWhenUnpublished)
{
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    CHECK_EQUAL(0, msgbus_subscriber_has_update(&sub));
}

TEST(SubscriberTests, SubscriberHasUpdateAfterPublish)
{
    simple_t val;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    msgbus_topic_publish(&topic, &val);
    msgbus_topic_publish(&topic, &val);
    CHECK_EQUAL(2, msgbus_subscriber_has_update(&sub));
}

TEST(SubscriberTests, SubscriberUpdateCountResetsAfterRead)
{
    simple_t val;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    msgbus_topic_publish(&topic, &val);
    msgbus_topic_publish(&topic, &val);
    msgbus_subscriber_read(&sub, &val);
    CHECK_EQUAL(0, msgbus_subscriber_has_update(&sub));
}

TEST(SubscriberTests, SubscriberReturnsNbrOfUpdatesOnRead)
{
    simple_t val;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    msgbus_topic_publish(&topic, &val);
    msgbus_topic_publish(&topic, &val);
    int ret = msgbus_subscriber_read(&sub, &val);
    CHECK_EQUAL(2, ret);
}

TEST(SubscriberTests, SubscriberUpdateCountIsExactlyOneIfSubscribedAfterPublish)
{
    simple_t val;
    topic.pub_seq_nbr = 1234;
    msgbus_topic_publish(&topic, &val);
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    CHECK_EQUAL(1, msgbus_subscriber_has_update(&sub));

    topic.pub_seq_nbr = 0; // even in case of overflow
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    CHECK_EQUAL(1, msgbus_subscriber_has_update(&sub));
}

TEST(SubscriberTests, HasUpdateHandlesPublishSequenceOverflowCorrectly)
{
    simple_t val;
    msgbus_topic_publish(&topic, &val);
    topic.pub_seq_nbr = UINT32_MAX; // start before overflow
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    msgbus_subscriber_read(&sub, &val); // get subscriber to same count
    topic.pub_seq_nbr = 2;
    CHECK_EQUAL(3, msgbus_subscriber_has_update(&sub));
    topic.pub_seq_nbr = UINT32_MAX-1;
    CHECK_EQUAL(UINT32_MAX, msgbus_subscriber_has_update(&sub));
}

TEST(SubscriberTests, WaitForUpdateReturnsFalseWithTimeoutImmediateNoUpdate)
{
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    CHECK_FALSE(msgbus_subscriber_wait_for_update(&sub, MSGBUS_TIMEOUT_IMMEDIATE));
}

TEST(SubscriberTests, WaitForUpdateReturnsTrueIfUpdate)
{
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    simple_t val;
    msgbus_topic_publish(&topic, &val);
    CHECK_TRUE(msgbus_subscriber_wait_for_update(&sub, MSGBUS_TIMEOUT_IMMEDIATE));
}

TEST(SubscriberTests, SubscriberGetTopic)
{
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    POINTERS_EQUAL(&topic, msgbus_subscriber_get_topic(&sub));
}

TEST(SubscriberTests, PublishAndReadBack)
{
    CHECK_TRUE(msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE));
    simple_t t = {42};
    msgbus_topic_publish(&topic, &t);
    simple_t s;
    msgbus_subscriber_read(&sub, &s);
    CHECK_EQUAL(42, s.x);
}

TEST(SubscriberTests, ReadJustUpdatesTheSeqNbrIfDestIsNULL)
{
    CHECK_TRUE(msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE));
    simple_t t = {42};
    msgbus_topic_publish(&topic, &t);
    msgbus_subscriber_read(&sub, NULL);
    CHECK_FALSE(msgbus_subscriber_has_update(&sub));
}
