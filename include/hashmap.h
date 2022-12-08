#include <stdlib.h>
#include <stdarg.h>
#include <check.h>


enum hash_status {
    HASH_OK,
    HASH_NOT_FOUND
};


#define HASHMAP_CONTEXT(KTYPE, VTYPE, SUFFIX)           \
                                                        \
    struct hashmap_value_node_##SUFFIX                  \
    {                                                   \
        VTYPE value;                                    \
        KTYPE key;                                      \
        struct hashmap_value_node_##SUFFIX* next;       \
    };                                                  \
                                                        \
    struct hashmap_context_##SUFFIX                     \
    {                                                   \
        struct hashmap_value_node_##SUFFIX** buckets;   \
        int count;                                      \
        int capacity;                                   \
        int (*hash_code)(KTYPE val);                    \
        int (*equals)(KTYPE val1, KTYPE val2);          \
    };                                                  \


#define HASHMAP_INIT(KTYPE, VTYPE, SUFFIX)                                                                                    \
    struct hashmap_context_##SUFFIX hashmap_init_##SUFFIX(int (*hash_code)(KTYPE val), int (*equals)(KTYPE val1, KTYPE val2)) \
    {                                                                                                                         \
        struct hashmap_context_##SUFFIX ctx;                                                                                  \
        ctx.count = 0;                                                                                                        \
        ctx.capacity = 10;                                                                                                    \
        ctx.buckets = (struct hashmap_value_node_##SUFFIX*)calloc(10, sizeof(struct hashmap_value_node_##SUFFIX*));            \
        ctx.hash_code = hash_code;                                                                                            \
        ctx.equals = equals;                                                                                                  \
                                                                                                                              \
        return ctx;                                                                                                           \
    }                                                                                                                         \
                                                                                                                                         \
    struct hashmap_context_##SUFFIX hashmap_init3_##SUFFIX(int (*hash_code)(KTYPE val), int (*equals)(KTYPE val1, KTYPE val2), int size) \
    {                                                                                                                                    \
        struct hashmap_context_##SUFFIX ctx;                                                                                             \
        ctx.count = 0;                                                                                                                   \
        ctx.capacity = size;                                                                                                             \
        ctx.buckets = (struct hashmap_value_node_##SUFFIX*)calloc(size, sizeof(struct hashmap_value_node_##SUFFIX*));                     \
        ctx.hash_code = hash_code;                                                                                                       \
        ctx.equals = equals;                                                                                                             \
                                                                                                                                         \
        return ctx;                                                                                                                      \
    }                                                                                                                                    \



#define HASHMAP_PUT(KTYPE, VTYPE, SUFFIX)                                                                                                \
    void hashmap_put_##SUFFIX(struct hashmap_context_##SUFFIX* ctx, KTYPE key, VTYPE value)                                              \
    {                                                                                                                                    \
        if(ctx->count == ctx->capacity)                                                                                                  \
        {                                                                                                                                \
             struct hashmap_context_##SUFFIX newCtx = hashmap_init3_##SUFFIX(ctx->hash_code, &ctx->equals, 2*ctx->capacity);            \
                                                                                                                                         \
             for(int i=0;i<ctx->capacity;i++)                                                                                            \
             {                                                                                                                           \
                if(ctx->buckets[i] != NULL)                                                                                              \
                {                                                                                                                        \
                    struct hashmap_value_node_##SUFFIX* node = ctx->buckets[i];                                                                 \
                    while(node != NULL)                                                                                                  \
                    {                                                                                                                    \
                        hashmap_put_##SUFFIX(&newCtx, node->key, node->value);                                                           \
                        node = node->next;                                                                                               \
                    }                                                                                                                    \
                }                                                                                                                        \
             }                                                                                                                           \
                                                                                                                                         \
             *ctx = newCtx;                                                                                                              \
        }                                                                                                                                \
                                                                                                                                         \
        int hash = ctx->hash_code(key) % ctx->capacity;                                                                                  \
                                                                                                                                         \
        if(ctx->buckets[hash] == NULL)                                                                                                   \
        {                                                                                                                                \
            struct hashmap_value_node_##SUFFIX* node = (struct hashmap_value_node_##SUFFIX*)malloc(sizeof(struct hashmap_value_node_##SUFFIX));      \
            node->key = key;                                                                                                             \
            node->value = value;                                                                                                         \
            node->next = NULL;                                                                                                           \
                                                                                                                                         \
            ctx->buckets[hash] = node;                                                                                                   \
            ctx->count++;                                                                                                                \
            return;                                                                                                                      \
        }                                                                                                                                \
                                                                                                                                         \
        int found = 0;                                                                                                                   \
        struct hashmap_value_node_##SUFFIX* prev = NULL;                                                                                 \
        struct hashmap_value_node_##SUFFIX* it = ctx->buckets[hash];                                                                     \
        while(it != NULL)                                                                                                      \
        {                                                                                                                                \
            if(ctx->equals(key, it->key))                                                                                                \
            {                                                                                                                            \
                found = 1;                                                                                                               \
                break;                                                                                                                   \
            }                                                                                                                            \
            else                                                                                                                         \
            {                                                                                                                            \
                prev = it;                                                                                                               \
                it = it->next;                                                                                                           \
            }                                                                                                                            \
        }                                                                                                                                \
                                                                                                                                         \
        if(!found)                                                                                                                       \
        {                                                                                                                                \
           struct hashmap_value_node_##SUFFIX* node = (struct hashmap_value_node_##SUFFIX*)malloc(sizeof(struct hashmap_value_node_##SUFFIX));    \
           node->key = key;                                                                                                             \
           node->value = value;                                                                                                         \
           node->next = NULL;                                                                                                           \
                                                                                                                                        \
           prev->next = node;                                                                                                           \
           ctx->count++;                                                                                                                \
        }                                                                                                                               \
                                                                                                                                        \
    }                                                                                                                                   \


#define HASHMAP_DEFINE_ALL(KTYPE, VTYPE, SUFFIX)        \
    HASHMAP_CONTEXT(KTYPE, VTYPE, SUFFIX)               \
    HASHMAP_INIT(KTYPE, VTYPE, SUFFIX)                  \
    HASHMAP_PUT(KTYPE, VTYPE, SUFFIX)