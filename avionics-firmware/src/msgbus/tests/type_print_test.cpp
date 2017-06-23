#include <CppUTest/TestHarness.h>
#include <inttypes.h>
#include "../type_definition.h"
#include "../type_print.h"
#include "types/test.h"

extern "C"
void print_fn(void *p, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char **buf = (char **)p;
    int n = vsprintf(*buf, fmt, ap);
    if (n > 0) {
        *buf += n;
    }
    va_end(ap);
}

TEST_GROUP(TypePrintTestGroup)
{
    char buffer[1000];
    void *arg;

    void setup(void)
    {
        memset(buffer, 0, sizeof(buffer));
        arg = &buffer;
    }
};

TEST(TypePrintTestGroup, CanPrintInt32)
{
    bool ret;
    msgbus_type_entry_t entry = {
        .name = "int",
        .is_base_type = 1,
        .is_array = 0,
        .is_dynamic_array = 0,
        .base_type = MSGBUS_TYPE_INT32,
        .struct_offset = 0,
    };
    int32_t object = 123;
    ret = msgbus_print_entry(print_fn, &arg, &entry, &object, 0);
    CHECK_TRUE(ret)
    STRCMP_EQUAL("int: 123\n", buffer);
}

TEST(TypePrintTestGroup, CanPrintFloat32)
{
    bool ret;
    msgbus_type_entry_t entry = {
        .name = "float",
        .is_base_type = 1,
        .is_array = 0,
        .is_dynamic_array = 0,
        .base_type = MSGBUS_TYPE_FLOAT32,
        .struct_offset = 0,
    };
    float object = 2.5;
    ret = msgbus_print_entry(print_fn, &arg, &entry, &object, 0);
    CHECK_TRUE(ret)
    STRCMP_EQUAL("float: 2.500000\n", buffer);
}

TEST(TypePrintTestGroup, CanPrintString)
{
    bool ret;
    msgbus_type_entry_t entry = {
        .name = "string",
        .is_base_type = 1,
        .is_array = 0,
        .is_dynamic_array = 0,
        .base_type = MSGBUS_TYPE_STRING,
        .struct_offset = 0,
        .size = 5,
    };
    const char object[] = "CVRA";
    ret = msgbus_print_entry(print_fn, &arg, &entry, &object, 0);
    CHECK_TRUE(ret)
    STRCMP_EQUAL("string: \"CVRA\"\n", buffer);
}

TEST(TypePrintTestGroup, CanPrintSimpleType)
{
    simple_t object = {.x = 1.5f, .y = 42};
    msgbus_print_type(print_fn, &arg, &simple_type, &object);
    STRCMP_EQUAL(
        "x: 1.500000\n"
        "y: 42\n",
        buffer);
}

TEST(TypePrintTestGroup, CanPrintNestedType)
{
    nested_t object = {.s = {.x = 1.0, .y = 123}};
    msgbus_print_type(print_fn, &arg, &nested_type, &object);
    STRCMP_EQUAL(
        "s:\n"
        "    x: 1.000000\n"
        "    y: 123\n",
        buffer);
}

TEST(TypePrintTestGroup, CanPrintArrayType)
{
    arrays_t object = {.x = {1,2,0,0}, .x_len = 2, .strings = {"hello", "world"}};
    msgbus_print_type(print_fn, &arg, &arrays_type, &object);
    STRCMP_EQUAL(
        "x: [1, 2]\n"
        "strings: [\"hello\", \"world\"]\n",
        buffer);
}

TEST(TypePrintTestGroup, CanPrintArraysOfStructs)
{
    arrays_of_structs_t object = {.arr = {
        {.x = 0.0f, .y = 0},
        {.x = 1.0f, .y = 1},
        {.x = 2.0f, .y = 2}
    }};
    msgbus_print_type(print_fn, &arg, &arrays_of_structs_type, &object);
    STRCMP_EQUAL(
        "arr: [\n"
        "    x: 0.000000\n"
        "    y: 0\n"
        "\n"
        "    x: 1.000000\n"
        "    y: 1\n"
        "\n"
        "    x: 2.000000\n"
        "    y: 2\n"
        "]\n",
        buffer);
}

TEST(TypePrintTestGroup, CanPrintAllIntegerTypes)
{
    integer_types_t object = {
        .i8 = INT8_MIN,
        .i16 = INT16_MIN,
        .i32 = INT32_MIN,
        .i64 = INT64_MIN,
        .u8 = UINT8_MAX,
        .u16 = UINT16_MAX,
        .u32 = UINT32_MAX,
        .u64 = UINT64_MAX,
    };
    msgbus_print_type(print_fn, &arg, &integer_types_type, &object);
    char expect[1000];
    sprintf(expect,
        "i8: %" PRIi8 "\n"
        "i16: %" PRIi16 "\n"
        "i32: %" PRIi32 "\n"
        "i64: %" PRIi64 "\n"
        "u8: %" PRIu8 "\n"
        "u16: %" PRIu16 "\n"
        "u32: %" PRIu32 "\n"
        "u64: %" PRIu64 "\n",
        (int8_t)INT8_MIN,
        (int16_t)INT16_MIN,
        INT32_MIN,
        INT64_MIN,
        (uint8_t)UINT8_MAX,
        (uint16_t)UINT16_MAX,
        UINT32_MAX,
        UINT64_MAX);
    STRCMP_EQUAL(
        expect,
        buffer);
}
