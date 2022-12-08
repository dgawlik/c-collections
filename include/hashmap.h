#include <stdlib.h>
#include <stdarg.h>
#include <check.h>


enum vector_status {
    HASH_OK,
    HASH_NOT_FOUND
};



#define HASHMAP_CONTEXT(KTYPE, VTYPE, SUFFIX)           \
                                                        \
    struct hashmap_value_node_##SUFFIX                  \
    {                                                   \
        VTYPE value;                                    \
        struct hashmap_value_node_##SUFFIX* next;       \
    };                                                  \
                                                        \
    struct hashmap_context_##SUFFIX                     \
    {                                                   \
        KTYPE* keys;                                    \
        struct hashmap_value_node_##SUFFIX* buckets;    \
        int length;                                     \
        int capacity;                                   \
        int (*hash_code)(KTYPE val);                    \
        int (*equals)(VTYPE val1, VTYPE val2);          \
    };                                                  \


#define HASHMAP_INIT(KTYPE, VTYPE, SUFFIX)                                                                                    \
    struct hashmap_context_##SUFFIX hashmap_init_##SUFFIX(int (*hash_code)(KTYPE val), int (*equals)(VTYPE val1, VTYPE val2)) \
    {                                                                                                                         \
        struct hashmap_context_##SUFFIX ctx;                                                                                  \
        ctx.length = 0;                                                                                                       \
        ctx.capacity = 10;                                                                                                    \
        ctx.keys = (KTYPE*)malloc(10*sizeof(KTYPE));                                                                          \
        ctx.buckets = (struct hashmap_value_node_##SUFFIX*)malloc(10*sizeof(struct hashmap_value_node_##SUFFIX));             \
        ctx.hash_code = hash_code;                                                                                            \
        ctx.equals = equals;                                                                                                  \
                                                                                                                              \
        return ctx;                                                                                                           \
    }                                                                                                                         \


#define HASHMAP_DEFINE_ALL(KTYPE, VTYPE, SUFFIX)        \
    HASHMAP_CONTEXT(KTYPE, VTYPE, SUFFIX)               \
    HASHMAP_INIT(KTYPE, VTYPE, SUFFIX)