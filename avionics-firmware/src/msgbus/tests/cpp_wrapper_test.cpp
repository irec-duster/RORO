#include "types/test.h"
#include "mocks/assert.hpp"
#include "../typesafe_wrapper.h"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>


TEST_GROUP(WrapperTest)
{
    msgbus_t bus;

    void setup()
    {
        msgbus_init(&bus);
    }

    void teardown()
    {
        msgbus_assert_mock_enable(false);
        mock().checkExpectations();
        mock().clear();
    }
};


TEST(WrapperTest, TopicPublishAndSubscribe)
{
    msgbus::Topic<simple_t, &simple_type> t;
    t.advertise(&bus, "test");
    simple_t tx = {.x=42};
    t.publish(tx);

    msgbus::Subscriber<simple_t, &simple_type> s;
    s.subscribe(&bus, "test");
    CHECK_TRUE(s.has_update());
    simple_t rx;
    CHECK_EQUAL(1, s.read(rx));
    CHECK_EQUAL(rx.x, 42);
}

TEST(WrapperTest, TemplateHelperMacros)
{
    MsgBusTopic(simple) t;
    t.advertise(&bus, "test");
    simple_t tx = {.x=42};
    t.publish(tx);

    MsgBusSubscriber(simple) s;
    s.subscribe(&bus, "test");
    simple_t rx;
    s.read(rx);
    CHECK_EQUAL(rx.x, 42);
}


TEST(WrapperTest, TypeMissmatchIsDetected)
{
    msgbus::Topic<simple_t, &simple_type> t;
    t.advertise(&bus, "test");

    msgbus_assert_mock_enable(true);
    mock().expectOneCall("msgbus_unittest_assert")
          .withParameter("condition", false);

    msgbus::Subscriber<nested_t, &nested_type> s;
    s.subscribe(&bus, "test");
}
