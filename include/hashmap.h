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
        ctx.buckets = (struct hashmap_value_node_##SUFFIX**)calloc(10, sizeof(struct hashmap_value_node_##SUFFIX*));          \
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
        ctx.buckets = (struct hashmap_value_node_##SUFFIX**)calloc(size, sizeof(struct hashmap_value_node_##SUFFIX*));                   \
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
             struct hashmap_context_##SUFFIX newCtx = hashmap_init3_##SUFFIX(ctx->hash_code, ctx->equals, 2*ctx->capacity);              \
                                                                                                                                         \
             for(int i=0;i<ctx->capacity;i++)                                                                                            \
             {                                                                                                                           \
                if(ctx->buckets[i] != NULL)                                                                                              \
                {                                                                                                                        \
                    struct hashmap_value_node_##SUFFIX* node = ctx->buckets[i];                                                          \
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
            struct hashmap_value_node_##SUFFIX* node = (struct hashmap_value_node_##SUFFIX*)calloc(1, sizeof(struct hashmap_value_node_##SUFFIX));      \
            node->key = key;                                                                                                             \
            node->value = value;                                                                                                         \
            node->next = NULL;                                                                                                           \
                                                                                                                                         \
            ctx->buckets[hash] = node;                                                                                                   \
            ctx->count++;                                                                                                                \
            return;                                                                                                                      \
        }                                                                                                                                \
                                                                                                                                         \
        struct hashmap_value_node_##SUFFIX* prev = NULL;                                                                                 \
        struct hashmap_value_node_##SUFFIX* it = ctx->buckets[hash];                                                                     \
        while(it != NULL)                                                                                                                \
        {                                                                                                                                \
            if(ctx->equals(key, it->key))                                                                                                \
            {                                                                                                                            \
                it->value = value;                                                                                                       \
                return;                                                                                                                  \
            }                                                                                                                            \
                                                                                                                                         \
            prev = it;                                                                                                                   \
            it = it->next;                                                                                                               \
        }                                                                                                                                \
                                                                                                                                         \
        struct hashmap_value_node_##SUFFIX* node = (struct hashmap_value_node_##SUFFIX*)calloc(1, sizeof(struct hashmap_value_node_##SUFFIX));    \
        node->key = key;                                                                                                                 \
        node->value = value;                                                                                                             \
        node->next = NULL;                                                                                                               \
                                                                                                                                         \
        prev->next = node;                                                                                                               \
        ctx->count++;                                                                                                                    \
                                                                                                                                         \
    }                                                                                                                                    \

#define HASHMAP_GET(KTYPE, VTYPE, SUFFIX)                                                                   \
    enum hash_status hashmap_get_##SUFFIX(struct hashmap_context_##SUFFIX* ctx, KTYPE key, VTYPE* value)    \
    {                                                                                                       \
        int hash = ctx->hash_code(key) % ctx->capacity;                                                     \
                                                                                                            \
        if(ctx->buckets[hash] == NULL)                                                                      \
        {                                                                                                   \
            return HASH_NOT_FOUND;                                                                          \
        }                                                                                                   \
                                                                                                            \
        struct hashmap_value_node_##SUFFIX* it = ctx->buckets[hash];                                        \
        while(it != NULL)                                                                                   \
        {                                                                                                   \
            if(ctx->equals(key, it->key))                                                                   \
            {                                                                                               \
                *value = it->value;                                                                         \
                return HASH_OK;                                                                             \
            }                                                                                               \
                                                                                                            \
            it = it->next;                                                                                  \
        }                                                                                                   \
                                                                                                            \
        return HASH_NOT_FOUND;                                                                              \
    }                                                                                                       \




#define HASHMAP_REMOVE(KTYPE, VTYPE, SUFFIX)                                                                \
    enum hash_status hashmap_remove_##SUFFIX(struct hashmap_context_##SUFFIX* ctx, KTYPE key)               \
    {                                                                                                       \
        int hash = ctx->hash_code(key) % ctx->capacity;                                                     \
                                                                                                            \
        if(ctx->buckets[hash] == NULL)                                                                      \
        {                                                                                                   \
            return HASH_NOT_FOUND;                                                                          \
        }                                                                                                   \
                                                                                                            \
        struct hashmap_value_node_##SUFFIX* prev = NULL;                                                    \
        struct hashmap_value_node_##SUFFIX** it = &(ctx->buckets[hash]);                                    \
        while(*it != NULL)                                                                                  \
        {                                                                                                   \
            if(ctx->equals(key, (*it)->key))                                                                \
            {                                                                                               \
                if(prev != NULL)                                                                            \
                {                                                                                           \
                    prev->next = (*it)->next;                                                               \
                }                                                                                           \
                free((*it));                                                                                \
                *it = NULL;                                                                                 \
                return HASH_OK;                                                                             \
            }                                                                                               \
                                                                                                            \
            prev = *it;                                                                                     \
            it = &((*it)->next);                                                                            \
        }                                                                                                   \
                                                                                                            \
        return HASH_NOT_FOUND;                                                                              \
    }                                                                                                       \


#define HASHMAP_CONTAINS_KEY(KTYPE, VTYPE, SUFFIX)                                                          \
    int hashmap_contains_key(struct hashmap_context_##SUFFIX* ctx, KTYPE key)                               \
    {                                                                                                       \
        VTYPE value;                                                                                        \
        return hashmap_get_##SUFFIX(ctx, key, &value) == HASH_OK;                                           \
    }                                                                                                       \


#define HASHMAP_DEFINE_ALL(KTYPE, VTYPE, SUFFIX)        \
    HASHMAP_CONTEXT(KTYPE, VTYPE, SUFFIX)               \
    HASHMAP_INIT(KTYPE, VTYPE, SUFFIX)                  \
    HASHMAP_PUT(KTYPE, VTYPE, SUFFIX)                   \
    HASHMAP_GET(KTYPE, VTYPE, SUFFIX)                   \
    HASHMAP_REMOVE(KTYPE, VTYPE, SUFFIX)                \
    HASHMAP_CONTAINS_KEY(KTYPE, VTYPE, SUFFIX)