#ifndef MSGBUS_TYPE_DEFINITION_H
#define MSGBUS_TYPE_DEFINITION_H

#include <stdint.h>
#include <stddef.h>

#define MSGBUS_TYPE_HASH_SIZE 4

enum msgbus_base_type_enum {
    MSGBUS_TYPE_INT8,
    MSGBUS_TYPE_INT16,
    MSGBUS_TYPE_INT32,
    MSGBUS_TYPE_INT64,
    MSGBUS_TYPE_UINT8,
    MSGBUS_TYPE_UINT16,
    MSGBUS_TYPE_UINT32,
    MSGBUS_TYPE_UINT64,
    MSGBUS_TYPE_FLOAT16,
    MSGBUS_TYPE_FLOAT32,
    MSGBUS_TYPE_FLOAT64,
    MSGBUS_TYPE_STRING
};


typedef struct msgbus_type_definition_s msgbus_type_definition_t;
typedef struct msgbus_type_entry_s msgbus_type_entry_t;

struct msgbus_type_definition_s {
    uint8_t hash[MSGBUS_TYPE_HASH_SIZE];
    uint16_t nb_elements;
    const msgbus_type_entry_t *elements;
    size_t struct_size;
};

struct msgbus_type_entry_s {
    const char *name;
    union {
        enum msgbus_base_type_enum base_type;
        const msgbus_type_definition_t *type;
    };
    uint8_t is_base_type:1;
    uint8_t is_array:1;
    uint8_t is_dynamic_array:1;
    uint16_t array_len; // or max length for dynamic arrays
    size_t dynamic_array_len_struct_offset;
    size_t struct_offset;
    size_t size;
};

#endif /* MSGBUS_TYPE_DEFINITION_H */