#include <assert.h>
#include <CppUTestExt/MockSupport.h>

bool mock_assert;

void msgbus_assert_mock_enable(bool en)
{
    mock_assert = en;
}

void msgbus_unittest_assert(bool condition)
{
    if (mock_assert) {
        mock().actualCall("msgbus_unittest_assert")
              .withParameter("condition", condition);
    } else {
        assert(condition);
    }
}
