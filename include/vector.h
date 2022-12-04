#include <stdlib.h>
#include <stdarg.h>
#include <check.h>


enum vector_status {
    OK,
    OUT_OF_BOUNDS
};

#define VECTOR_NORETURN __attribute__ ((noreturn))

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


#define VECTOR_ADD(TYPE, SUFFIX)                                                 \
    void vector_add(struct vector_context_##SUFFIX* ctx, TYPE elem)              \
    {                                                                            \
        if(ctx->length == ctx->capacity)                                         \
        {                                                                        \
            TYPE* newArray = (TYPE*) malloc(sizeof(TYPE)*ctx->capacity*2);       \
            memcpy(newArray, ctx->array, sizeof(TYPE)*ctx->capacity);            \
            free(ctx->array);                                                    \
            ctx->array = newArray;                                               \
            ctx->capacity *= 2;                                                  \
        }                                                                        \
                                                                                 \
        ctx->array[ctx->length] = elem;                                          \
        ctx->length++;                                                           \
    }

#define VECTOR_DEFINE_ALL(TYPE, SUFFIX)                                  \
    VECTOR_CONTEXT(TYPE, SUFFIX)                                         \
    VECTOR_INIT(TYPE, SUFFIX)                                            \
    VECTOR_DESTROY(TYPE, SUFFIX)                                         \
    VECTOR_CHECK_BOUNDS(TYPE, SUFFIX)                                    \
    VECTOR_ADD(TYPE, SUFFIX)                                             \
    VECTOR_ASSERT(TYPE, SUFFIX)
