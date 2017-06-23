#include "../msgbus.h"
#include "mocks/synchronization.hpp"
#include "types/test.h"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


TEST_GROUP(TopicTests)
{
    msgbus_t bus;
    msgbus_topic_t topic;
    simple_t buffer;

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



TEST(TopicTests, Initializer)
{
    memset(&topic, 0xff, sizeof(topic));
    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");

    POINTERS_EQUAL(&buffer, topic.buffer);
    POINTERS_EQUAL(&simple_type, topic.type);
    POINTERS_EQUAL(&bus, topic.bus);
    POINTERS_EQUAL(NULL, topic.waiting_threads);
    CHECK_EQUAL(0, topic.pub_seq_nbr);
    STRCMP_EQUAL("topic", topic.name);
}

TEST(TopicTests, PublishCopiesData)
{
    simple_t data = {42};

    msgbus_topic_publish(&topic, &data);

    MEMCMP_EQUAL(topic.buffer, &data, sizeof(data));
}

TEST(TopicTests, PublishIncrementsSequenceNbr)
{
    simple_t data = {42};

    msgbus_topic_publish(&topic, &data);

    CHECK_EQUAL(1, topic.pub_seq_nbr);
}

TEST(TopicTests, PublishSequenceNbrCorrectlyOverflows)
{
    simple_t data = {42};

    topic.pub_seq_nbr = UINT32_MAX;
    msgbus_topic_publish(&topic, &data);

    CHECK_EQUAL(0, topic.pub_seq_nbr);
}

TEST(TopicTests, GetTopicType)
{
    POINTERS_EQUAL(&simple_type, msgbus_topic_get_type(&topic));
}

TEST(TopicTests, GetTopicName)
{
    STRCMP_EQUAL("topic", msgbus_topic_get_name(&topic));
}

