#include "../msgbus.h"
#include "mocks/synchronization.hpp"
#include "types/test.h"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>



extern "C" {
void _msgbus_cond_link_to_topic(struct msgbus_cond_link_s *link,
                               msgbus_cond_t *cv,
                               msgbus_topic_t *topic);
void _msgbus_cond_unlink_from_topic(struct msgbus_cond_link_s *link,
                                   msgbus_topic_t *topic);
void _msgbus_topic_signal_all(msgbus_topic_t *topic);
}


TEST_GROUP(SignalingTestGroup)
{
    msgbus_t bus;
    msgbus_topic_t topic;
    msgbus_topic_t topic2;
    simple_t buffer;

    void setup()
    {
        mock().strictOrder();
        msgbus_init(&bus);
        msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");
        msgbus_topic_create(&topic2, &bus, &simple_type, &buffer, "topic2");
    }

    void teardown()
    {
        clear_condvar_wait_side_effect();
        condvar_mocks_ignore_cv_pointer_arg(false);
        condvar_init_mock_enable(false);
        lock_mocks_enable(false);
        condvar_mocks_enable(false);
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(SignalingTestGroup, TopicFindWithTimeoutImmediateDoesntBlockOnCond)
{
    condvar_mocks_enable(true);

    msgbus_find_topic(&bus, "notatopic", MSGBUS_TIMEOUT_IMMEDIATE);
}

TEST(SignalingTestGroup, TopicPublishNoBlockingSubscribers)
{
    lock_mocks_enable(true);
    condvar_mocks_enable(true);

    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &topic.bus->topic_update_lock);
    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &topic.bus->topic_update_lock);

    msgbus_topic_publish(&topic, &buffer);
}


TEST(SignalingTestGroup, TopicPublishWithBlockingSubscribers)
{
    lock_mocks_enable(true);
    condvar_mocks_enable(true);

    msgbus_cond_t cv1, cv2;
    struct msgbus_cond_link_s link1, link2;
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);
    _msgbus_cond_link_to_topic(&link2, &cv2, &topic);

    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &topic.bus->topic_update_lock);

    mock().expectOneCall("msgbus_condvar_broadcast")
          .withParameter("cond", &cv2);
    mock().expectOneCall("msgbus_condvar_broadcast")
          .withParameter("cond", &cv1);

    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &topic.bus->topic_update_lock);

    msgbus_topic_publish(&topic, &buffer);
}

TEST(SignalingTestGroup, SubscriberWaitWithTimeoutImmediateDoesntBlockOnCond)
{
    msgbus_subscriber_t sub;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);

    condvar_init_mock_enable(true);
    condvar_mocks_enable(true);
    CHECK_FALSE(msgbus_subscriber_wait_for_update(&sub, MSGBUS_TIMEOUT_IMMEDIATE));
}

TEST(SignalingTestGroup, SubscriberWaitBlocksOnCond)
{
    msgbus_subscriber_t sub;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);

    condvar_init_mock_enable(true);
    condvar_mocks_enable(true);
    condvar_mocks_ignore_cv_pointer_arg(true);
    mock().expectOneCall("msgbus_condvar_init");
    mock().expectOneCall("msgbus_condvar_wait")
          .withParameter("mutex", &topic.bus->topic_update_lock)
          .withParameter("timeout_us", 100);

    CHECK_FALSE(msgbus_subscriber_wait_for_update(&sub, 100));
}

TEST(SignalingTestGroup, SubscriberWaitWithPublishDuringWaitReturnsTrue)
{
    msgbus_subscriber_t sub;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);

    set_condvar_wait_side_effect([this](){msgbus_topic_publish(&topic, &buffer);});

    CHECK_TRUE(msgbus_subscriber_wait_for_update(&sub, 100));
}

TEST(SignalingTestGroup, SubscriberWaitWithPublishDuringWaitIsSignaled)
{
    msgbus_subscriber_t sub;
    msgbus_topic_subscribe(&sub, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);

    condvar_init_mock_enable(true);
    condvar_mocks_enable(true);
    condvar_mocks_ignore_cv_pointer_arg(true);

    mock().expectOneCall("msgbus_condvar_init");
    mock().expectOneCall("msgbus_condvar_wait")
          .withParameter("mutex", &topic.bus->topic_update_lock)
          .withParameter("timeout_us", 100);

    set_condvar_wait_side_effect([this](){msgbus_topic_publish(&topic, &buffer);});

    mock().expectOneCall("msgbus_condvar_broadcast");

    msgbus_subscriber_wait_for_update(&sub, 100);
}

TEST(SignalingTestGroup, SubscriberWaitOnAnyWithPublishDuringWaitIsSignaledAndReturnsTrue)
{
    msgbus_subscriber_t sub1;
    msgbus_topic_subscribe(&sub1, &bus, "topic", MSGBUS_TIMEOUT_IMMEDIATE);
    msgbus_subscriber_t sub2;
    msgbus_topic_subscribe(&sub2, &bus, "topic2", MSGBUS_TIMEOUT_IMMEDIATE);

    condvar_init_mock_enable(true);
    condvar_mocks_enable(true);
    condvar_mocks_ignore_cv_pointer_arg(true);

    mock().expectOneCall("msgbus_condvar_init");
    mock().expectOneCall("msgbus_condvar_wait")
          .withParameter("mutex", &topic.bus->topic_update_lock)
          .withParameter("timeout_us", 100);

    set_condvar_wait_side_effect([this](){msgbus_topic_publish(&topic, &buffer);});

    mock().expectOneCall("msgbus_condvar_broadcast");

    msgbus_subscriber_t *sub_list[] = {&sub1, &sub2};

    CHECK_TRUE(msgbus_subscriber_wait_for_update_on_any(sub_list, 2, 100));
}


TEST(SignalingTestGroup, Advertise)
{
    lock_mocks_enable(true);
    condvar_mocks_enable(true);

    mock().expectOneCall("msgbus_mutex_acquire")
          .withParameter("mutex", &bus.lock);

    mock().expectOneCall("msgbus_condvar_broadcast")
          .withParameter("cond", &bus.condvar);

    mock().expectOneCall("msgbus_mutex_release")
          .withParameter("mutex", &bus.lock);

    msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");
}



TEST_GROUP(CondVarLinkTests)
{
    msgbus_t bus;
    msgbus_topic_t topic;
    simple_t buffer;
    struct msgbus_cond_link_s link1;
    struct msgbus_cond_link_s link2;
    struct msgbus_cond_link_s link3;
    msgbus_cond_t cv1;
    msgbus_cond_t cv2;
    msgbus_cond_t cv3;

    void setup()
    {
        msgbus_init(&bus);
        msgbus_topic_create(&topic, &bus, &simple_type, &buffer, "topic");
    }

    void teardown()
    {
        condvar_mocks_enable(false);
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(CondVarLinkTests, TestLinkSingle)
{
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);

    POINTERS_EQUAL(&link1, topic.waiting_threads)
    POINTERS_EQUAL(&cv1, link1.cond);
    POINTERS_EQUAL(NULL, link1.prev);
    POINTERS_EQUAL(NULL, link1.next);
}

TEST(CondVarLinkTests, TestLinkMultiple)
{
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);
    _msgbus_cond_link_to_topic(&link2, &cv2, &topic);
    _msgbus_cond_link_to_topic(&link3, &cv3, &topic);

    POINTERS_EQUAL(&link3, topic.waiting_threads)
    POINTERS_EQUAL(NULL, link3.prev);
    POINTERS_EQUAL(&link2, link3.next);
    POINTERS_EQUAL(&link3, link2.prev);
    POINTERS_EQUAL(&link1, link2.next);
    POINTERS_EQUAL(&link2, link1.prev);
    POINTERS_EQUAL(NULL, link1.next);
}

TEST(CondVarLinkTests, TestUnlinkSingle)
{
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);
    _msgbus_cond_unlink_from_topic(&link1, &topic);

    POINTERS_EQUAL(NULL, topic.waiting_threads);
}

TEST(CondVarLinkTests, TestUnlinkFirst)
{
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);
    _msgbus_cond_link_to_topic(&link2, &cv2, &topic);
    _msgbus_cond_link_to_topic(&link3, &cv3, &topic);
    _msgbus_cond_unlink_from_topic(&link1, &topic);

    POINTERS_EQUAL(&link3, topic.waiting_threads);
    POINTERS_EQUAL(NULL, link2.next);
}

TEST(CondVarLinkTests, TestUnlinkCenter)
{
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);
    _msgbus_cond_link_to_topic(&link2, &cv2, &topic);
    _msgbus_cond_link_to_topic(&link3, &cv3, &topic);
    _msgbus_cond_unlink_from_topic(&link2, &topic);

    POINTERS_EQUAL(&link3, topic.waiting_threads);
    POINTERS_EQUAL(&link1, link3.next);
    POINTERS_EQUAL(&link3, link1.prev);
}

TEST(CondVarLinkTests, TestUnlinkLast)
{
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);
    _msgbus_cond_link_to_topic(&link2, &cv2, &topic);
    _msgbus_cond_link_to_topic(&link3, &cv3, &topic);
    _msgbus_cond_unlink_from_topic(&link3, &topic);

    POINTERS_EQUAL(&link2, topic.waiting_threads);
    POINTERS_EQUAL(NULL, link2.prev);
}

TEST(CondVarLinkTests, TestUnlinkAll)
{
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);
    _msgbus_cond_link_to_topic(&link2, &cv2, &topic);
    _msgbus_cond_link_to_topic(&link3, &cv3, &topic);
    _msgbus_cond_unlink_from_topic(&link1, &topic);
    _msgbus_cond_unlink_from_topic(&link2, &topic);
    _msgbus_cond_unlink_from_topic(&link3, &topic);

    POINTERS_EQUAL(NULL, topic.waiting_threads);
}

TEST(CondVarLinkTests, TestSignalEmpty)
{
    condvar_mocks_enable(true);
    _msgbus_topic_signal_all(&topic);
}

TEST(CondVarLinkTests, TestSignalAll)
{
    _msgbus_cond_link_to_topic(&link1, &cv1, &topic);
    _msgbus_cond_link_to_topic(&link2, &cv2, &topic);
    _msgbus_cond_link_to_topic(&link3, &cv3, &topic);

    mock().expectOneCall("msgbus_condvar_broadcast")
          .withParameter("cond", &cv1);
    mock().expectOneCall("msgbus_condvar_broadcast")
          .withParameter("cond", &cv2);
    mock().expectOneCall("msgbus_condvar_broadcast")
          .withParameter("cond", &cv3);

    condvar_mocks_enable(true);
    _msgbus_topic_signal_all(&topic);
}
