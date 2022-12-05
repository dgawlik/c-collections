#include <stdlib.h>
#include <stdarg.h>
#include <check.h>


enum vector_status {
    OK,
    OUT_OF_BOUNDS
};


#define VECTOR_ASSERT(TYPE, SUFFIX)                                                 \
    int is_vector_equal(struct vector_context_##SUFFIX* ctx, int va_count, ...)     \
    {                                                                               \
        va_list ap;                                                                 \
        va_start(ap, va_count);                                                     \
        for(int i=0; i<va_count; i++)                                               \
            if(ctx->array[i] != va_arg(ap, TYPE))                                   \
                return 0;                                                           \
        va_end(ap);                                                                 \
        return 1;                                                                   \
    };                                                                              \

#define VECTOR_CONTEXT(TYPE, SUFFIX) \
    struct vector_context_##SUFFIX   \
    {                                \
        TYPE *array;                 \
        size_t length;               \
        size_t capacity;             \
    };                               \


#define VECTOR_INIT(TYPE, SUFFIX)                                        \
    struct vector_context_##SUFFIX vector_init1_##SUFFIX(size_t capacity)\
    {                                                                    \
        struct vector_context_##SUFFIX ctx;                              \
        ctx.array = (TYPE *)malloc(sizeof(TYPE) * capacity);             \
        ctx.length = 0;                                                  \
        ctx.capacity = capacity;                                         \
                                                                         \
        return ctx;                                                      \
    };                                                                   \
                                                                         \
    struct vector_context_##SUFFIX vector_init0_##SUFFIX()               \
    {                                                                    \
        return vector_init1_##SUFFIX(10);                                \
    };                                                                   \

   


#define VECTOR_DESTROY(TYPE, SUFFIX)                                      \
    void vector_destroy_##SUFFIX(struct vector_context_##SUFFIX* ctx)     \
    {                                                                     \
        free(ctx->array);                                                 \
    };                                                                    \

#define VECTOR_CHECK_BOUNDS(TYPE, SUFFIX)                                       \
    int is_inside_bounds_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx) \
    {                                                                           \
        return idx >= 0 && idx < ctx->length;                                   \
    };                                                                          \

#define VECTOR_GROW(TYPE, SUFFIX)                                               \
    void vector_grow_##SUFFIX(struct vector_context_##SUFFIX* ctx)              \
    {                                                                           \
        TYPE* newArray = (TYPE*) malloc(sizeof(TYPE)*ctx->capacity*2);          \
        memcpy(newArray, ctx->array, sizeof(TYPE)*ctx->capacity);               \
        free(ctx->array);                                                       \
        ctx->array = newArray;                                                  \
        ctx->capacity *= 2;                                                     \
                                                                                \
    };                                                                          \

#define VECTOR_MOVE_RIGHT(TYPE, SUFFIX)                                         \
    void vector_move_right_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx)        \
    {                                                                           \
        for(int i=ctx->length-1;i>=idx;i--)                                     \
        {                                                                       \
            ctx->array[i+1] = ctx->array[i];                                    \
        }                                                                       \
    }                                                                           \


#define VECTOR_PUSH(TYPE, SUFFIX)                                                \
    void vector_push_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem)    \
    {                                                                            \
        if(ctx->length == ctx->capacity)                                         \
        {                                                                        \
            vector_grow_##SUFFIX(ctx);                                           \
        }                                                                        \
                                                                                 \
        ctx->array[ctx->length] = elem;                                          \
        ctx->length++;                                                           \
    }

#define VECTOR_INSERT(TYPE, SUFFIX)                                                                     \
    enum vector_status vector_insert_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem, int idx)  \
    {                                                                                                   \
        if(!is_inside_bounds_##SUFFIX(ctx, idx))                                                        \
        {                                                                                               \
            return OUT_OF_BOUNDS;                                                                       \
        }                                                                                               \
                                                                                                        \
        if(ctx->length == ctx->capacity)                                                                \
        {                                                                                               \
            vector_grow_##SUFFIX(ctx);                                                                  \
        }                                                                                               \
                                                                                                        \
        vector_move_right_##SUFFIX(ctx, idx);                                                           \
        ctx->array[idx] = elem;                                                                         \
        ctx->length++;                                                                                  \
                                                                                                        \
        return OK;                                                                                      \
    }                                                                                                   \





#define VECTOR_DEFINE_ALL(TYPE, SUFFIX)                                  \
    VECTOR_CONTEXT(TYPE, SUFFIX)                                         \
    VECTOR_INIT(TYPE, SUFFIX)                                            \
    VECTOR_DESTROY(TYPE, SUFFIX)                                         \
    VECTOR_CHECK_BOUNDS(TYPE, SUFFIX)                                    \
    VECTOR_GROW(TYPE, SUFFIX)                                            \
    VECTOR_MOVE_RIGHT(TYPE, SUFFIX)                                      \
    VECTOR_PUSH(TYPE, SUFFIX)                                            \
    VECTOR_INSERT(TYPE, SUFFIX)                                          \
    VECTOR_ASSERT(TYPE, SUFFIX)
