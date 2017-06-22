#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <CppUTest/TestHarness.h>
#include "../serialization_msgpack.h"
#include "types/test.h"

extern "C" {
#include "cmp/cmp.h"
}
#include "cmp_mem_access/cmp_mem_access.h"

extern "C" {
bool msgbus_cmp_ser_type(const void *var,
                         const msgbus_type_definition_t *type,
                         cmp_ctx_t *ctx,
                         bool compact);
bool msgbus_cmp_ser_struct_entry(const void *var,
                                 const msgbus_type_entry_t *entry,
                                 cmp_ctx_t *ctx,
                                 bool compact);
bool msgbus_cmp_ser_value(const void *var,
                          const msgbus_type_entry_t *entry,
                          cmp_ctx_t *ctx,
                          bool compact);
bool msgbus_cmp_ser_value_once(const void *var,
                               const msgbus_type_entry_t *entry,
                               cmp_ctx_t *ctx,
                               bool compact);
}



TEST_GROUP(MessagePackEntrySerializationTests)
{
    cmp_ctx_t ctx;
    cmp_mem_access_t mem;
    char buf[1000];
    msgbus_type_entry_t base_type_entry;
    void setup(void)
    {
        memset(buf, 0, sizeof(buf));
        cmp_mem_access_init(&ctx, &mem, buf, sizeof(buf));
        base_type_entry = {
            .name = "var",
            .base_type = MSGBUS_TYPE_INT32,
            .is_base_type = 1,
            .is_array = 0,
            .is_dynamic_array = 0,
            .array_len = 0,
            .dynamic_array_len_struct_offset = 0,
            .struct_offset = 0,
            .size = 0
        };
    }
};


TEST(MessagePackEntrySerializationTests, SerializeFloat16Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_FLOAT16;
    float var = 3.14;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    float var_read;
    CHECK_TRUE(cmp_read_float(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeFloat32Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_FLOAT32;
    float var = 3.14;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    float var_read;
    CHECK_TRUE(cmp_read_float(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeFloat64Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_FLOAT64;
    double var = 3.14;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    double var_read;
    CHECK_TRUE(cmp_read_double(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeInt8Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_INT8;
    int8_t var = INT8_MIN;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    int8_t var_read;
    CHECK_TRUE(cmp_read_char(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeInt16Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_INT16;
    int16_t var = INT16_MIN;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    int16_t var_read;
    CHECK_TRUE(cmp_read_short(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeInt32Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_INT32;
    int32_t var = INT32_MIN;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    int32_t var_read;
    CHECK_TRUE(cmp_read_int(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeInt64Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_INT64;
    int64_t var = INT64_MIN;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    int64_t var_read;
    CHECK_TRUE(cmp_read_long(&ctx, &var_read));
    CHECK_TRUE(var == var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeUInt8Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_UINT8;
    uint8_t var = UINT8_MAX;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    uint8_t var_read;
    CHECK_TRUE(cmp_read_uchar(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeUInt16Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_UINT16;
    uint16_t var = UINT16_MAX;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    uint16_t var_read;
    CHECK_TRUE(cmp_read_ushort(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeUInt32Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_UINT32;
    uint32_t var = UINT32_MAX;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    uint32_t var_read;
    CHECK_TRUE(cmp_read_uint(&ctx, &var_read));
    CHECK_EQUAL(var, var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeUInt64Value)
{
    base_type_entry.base_type = MSGBUS_TYPE_UINT64;
    uint64_t var = UINT64_MAX;
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    uint64_t var_read;
    CHECK_TRUE(cmp_read_ulong(&ctx, &var_read));
    CHECK_TRUE(var == var_read);
}

TEST(MessagePackEntrySerializationTests, SerializeStringValue)
{
    base_type_entry.base_type = MSGBUS_TYPE_STRING;
    base_type_entry.size = 10;
    char str[10] = "test";
    CHECK_TRUE(msgbus_cmp_ser_value(&str, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    memset(str, 0, sizeof(str));
    uint32_t len = sizeof(str);
    CHECK_TRUE(cmp_read_str(&ctx, &str[0], &len));
    STRCMP_EQUAL("test", str);
    CHECK_EQUAL(4, len);
}


TEST(MessagePackEntrySerializationTests, SerializeCustomTypeValue)
{
    msgbus_type_entry_t entry = {
        .is_base_type = 0,
        .is_array = 0,
        .is_dynamic_array = 0,
        .type = &simple_type,
        .struct_offset = 0,
    };
    simple_t var = {.x = 3.14, .y = 42};
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    uint32_t nb_elements;
    CHECK_TRUE(cmp_read_map(&ctx, &nb_elements));
    CHECK_EQUAL(2, nb_elements);
}


TEST(MessagePackEntrySerializationTests, SerializeStaticArray)
{
    base_type_entry.base_type = MSGBUS_TYPE_INT32;
    base_type_entry.is_array = 1;
    base_type_entry.array_len = 3;
    base_type_entry.size = sizeof(int32_t);
    int32_t var[3] = {1, 2, 3};
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &base_type_entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);

    uint32_t nb_elements;
    CHECK_TRUE(cmp_read_map(&ctx, &nb_elements));
    CHECK_EQUAL(3, nb_elements);
    int32_t var_read;
    CHECK_TRUE(cmp_read_int(&ctx, &var_read));
    CHECK_EQUAL(var[0], var_read);
    CHECK_TRUE(cmp_read_int(&ctx, &var_read));
    CHECK_EQUAL(var[1], var_read);
    CHECK_TRUE(cmp_read_int(&ctx, &var_read));
    CHECK_EQUAL(var[2], var_read);
}


TEST(MessagePackEntrySerializationTests, SerializeDynamicArray)
{
    struct dynamic_array_test_s {
        int32_t list[10];
        int list_len;
    };
    msgbus_type_entry_t entry = {
        .is_base_type = 1,
        .is_array = 1,
        .is_dynamic_array = 1,
        .base_type = MSGBUS_TYPE_INT32,
        .struct_offset = offsetof(dynamic_array_test_s, list),
        .dynamic_array_len_struct_offset = offsetof(dynamic_array_test_s, list_len),
        .array_len = 10,
        .size = sizeof(int32_t),
    };
    struct dynamic_array_test_s var = {.list = {1, 2, 3}, .list_len = 3};
    CHECK_TRUE(msgbus_cmp_ser_value(&var, &entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);

    uint32_t nb_elements;
    CHECK_TRUE(cmp_read_map(&ctx, &nb_elements));
    CHECK_EQUAL(3, nb_elements);
    int32_t var_read;
    CHECK_TRUE(cmp_read_int(&ctx, &var_read));
    CHECK_EQUAL(var.list[0], var_read);
    CHECK_TRUE(cmp_read_int(&ctx, &var_read));
    CHECK_EQUAL(var.list[1], var_read);
    CHECK_TRUE(cmp_read_int(&ctx, &var_read));
    CHECK_EQUAL(var.list[2], var_read);
}


TEST(MessagePackEntrySerializationTests, SerializeDynamicArrayMaxSizeCheck)
{
    struct dynamic_array_test_s {
        int32_t list[10];
        int list_len;
    };
    msgbus_type_entry_t entry = {
        .is_base_type = 1,
        .is_array = 1,
        .is_dynamic_array = 1,
        .base_type = MSGBUS_TYPE_INT32,
        .struct_offset = offsetof(dynamic_array_test_s, list),
        .dynamic_array_len_struct_offset = offsetof(dynamic_array_test_s, list_len),
        .array_len = 10,
        .size = sizeof(int32_t),
    };
    struct dynamic_array_test_s var = {.list_len = 11};
    CHECK_FALSE(msgbus_cmp_ser_value(&var, &entry, &ctx, false));
}


TEST(MessagePackEntrySerializationTests, SerializeStructEntry)
{
    msgbus_type_entry_t entry = {
        .name = "var",
        .is_base_type = 1,
        .is_array = 0,
        .is_dynamic_array = 0,
        .base_type = MSGBUS_TYPE_INT32,
        .struct_offset = 0,
    };
    int32_t var = 42;
    CHECK_TRUE(msgbus_cmp_ser_struct_entry(&var, &entry, &ctx, false));
    cmp_mem_access_set_pos(&mem, 0);
    char name_read_buf[10];
    uint32_t name_read_buf_sz = sizeof(name_read_buf);
    int32_t var_read;
    CHECK_TRUE(cmp_read_str(&ctx, name_read_buf, &name_read_buf_sz));
    CHECK_TRUE(cmp_read_int(&ctx, &var_read));
    STRCMP_EQUAL(entry.name, name_read_buf);
    CHECK_EQUAL(var, var_read);
}

// todo buffer too short tests

TEST_GROUP(MessagePackSerializationTests)
{
    cmp_ctx_t ctx;
    cmp_mem_access_t mem;
    char buf[1000];
    void setup(void)
    {
        memset(buf, 0, sizeof(buf));
        cmp_mem_access_init(&ctx, &mem, buf, sizeof(buf));
    }
};


TEST(MessagePackSerializationTests, SerializeStruct)
{
    simple_t val = {.x = 3.14, .y = 42};
    CHECK_TRUE(msgbus_cmp_ser_type(&val, &simple_type, &ctx, false));

    cmp_mem_access_set_pos(&mem, 0);
    uint32_t nb_elements;
    char name_read_buf[10];
    uint32_t name_read_buf_sz = sizeof(name_read_buf);
    float var_f_read;
    int32_t var_i_read;
    CHECK_TRUE(cmp_read_map(&ctx, &nb_elements));

    CHECK_EQUAL(2, nb_elements);
    CHECK_TRUE(cmp_read_str(&ctx, name_read_buf, &name_read_buf_sz));
    CHECK_TRUE(cmp_read_float(&ctx, &var_f_read));
    STRCMP_EQUAL("x", name_read_buf);
    CHECK_EQUAL(val.x, var_f_read);

    name_read_buf_sz = sizeof(name_read_buf);
    CHECK_TRUE(cmp_read_str(&ctx, name_read_buf, &name_read_buf_sz));
    CHECK_TRUE(cmp_read_int(&ctx, &var_i_read));
    STRCMP_EQUAL("y", name_read_buf);
    CHECK_EQUAL(val.y, var_i_read);
}


TEST(MessagePackSerializationTests, SerializeStructCompact)
{
    simple_t val = {.x = 3.14, .y = 42};
    CHECK_TRUE(msgbus_cmp_ser_type(&val, &simple_type, &ctx, true));

    cmp_mem_access_set_pos(&mem, 0);
    uint32_t nb_elements;
    float var_f_read;
    int32_t var_i_read;
    CHECK_TRUE(cmp_read_array(&ctx, &nb_elements));
    CHECK_EQUAL(2, nb_elements);
    CHECK_TRUE(cmp_read_float(&ctx, &var_f_read));
    CHECK_EQUAL(val.x, var_f_read);
    CHECK_TRUE(cmp_read_int(&ctx, &var_i_read));
    CHECK_EQUAL(val.y, var_i_read);
}

TEST(MessagePackSerializationTests, SerializeNestedStructCompact)
{
    nested_t val = {.s = {.x = 3.14, .y = 42}};
    CHECK_TRUE(msgbus_cmp_ser_type(&val, &nested_type, &ctx, true));

    cmp_mem_access_set_pos(&mem, 0);
    uint32_t nb_elements;
    float var_f_read;
    int32_t var_i_read;
    CHECK_TRUE(cmp_read_array(&ctx, &nb_elements));
    CHECK_EQUAL(1, nb_elements);
    CHECK_TRUE(cmp_read_array(&ctx, &nb_elements));
    CHECK_EQUAL(2, nb_elements);
    CHECK_TRUE(cmp_read_float(&ctx, &var_f_read));
    CHECK_EQUAL(val.s.x, var_f_read);
    CHECK_TRUE(cmp_read_int(&ctx, &var_i_read));
    CHECK_EQUAL(val.s.y, var_i_read);
}

