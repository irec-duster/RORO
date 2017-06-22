#ifndef MSGBUS_TYPE_PRINT_H
#define MSGBUS_TYPE_PRINT_H

#include "type_definition.h"

#ifdef __cplusplus
extern "C" {
#endif

bool msgbus_print_entry(void (*print_fn)(void *, const char *, ...),
                        void *arg,
                        const msgbus_type_entry_t *entry,
                        const void *object,
                        unsigned int indent);

bool msgbus_print_type_indent(void (*print_fn)(void *, const char *, ...),
                              void *arg,
                              const msgbus_type_definition_t *type,
                              const void *object,
                              unsigned int indent);

bool msgbus_print_type(void (*print_fn)(void *, const char *, ...),
                       void *arg,
                       const msgbus_type_definition_t *type,
                       const void *object);


#ifdef __cplusplus
}
#endif

#endif /* MSGBUS_TYPE_PRINT_H */
