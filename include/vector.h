#include <stdlib.h>
#include <stdarg.h>
#include <check.h>


enum vector_status {
    OK,
    OUT_OF_BOUNDS,
    EMPTY,
    NOT_FOUND
};


#define VECTOR_ASSERT(TYPE, SUFFIX)                                                 \
    int is_vector_equal_##SUFFIX(struct vector_context_##SUFFIX* ctx, int va_count, ...)     \
    {                                                                               \
        va_list ap;                                                                 \
        va_start(ap, va_count);                                                     \
        for(int i=0; i<va_count; i++)                                               \
            if(!vector_equals_##SUFFIX(ctx->array[i],va_arg(ap, TYPE)))             \
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


#define VECTOR_DEFAULT_EQUALS(TYPE, SUFFIX)                             \
    int vector_equals_##SUFFIX(TYPE lhs, TYPE rhs)                      \
    {                                                                   \
        return memcmp(&lhs, &rhs, sizeof(TYPE)) == 0;                   \
    }                                                                   \


#define VECTOR_DESTROY(TYPE, SUFFIX)                                      \
    void vector_destroy_##SUFFIX(struct vector_context_##SUFFIX* ctx)     \
    {                                                                     \
        free(ctx->array);                                                 \
    };                                                                    \
                                                                          \

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

#define VECTOR_MOVE_RIGHT(TYPE, SUFFIX)                                               \
    void vector_move_right_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx)     \
    {                                                                                 \
        for(int i=ctx->length-1;i>=idx;i--)                                           \
        {                                                                             \
            ctx->array[i+1] = ctx->array[i];                                          \
        }                                                                             \
    }                                                                                 \

#define VECTOR_MOVE_LEFT(TYPE, SUFFIX)                                              \
    void vector_move_left_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx)    \
    {                                                                               \
        for(int i=idx;i<ctx->length;i++)                                            \
        {                                                                           \
            ctx->array[i] = ctx->array[i+1];                                        \
        }                                                                           \
    }                                                                               \


#define VECTOR_PUSH_LAST(TYPE, SUFFIX)                                                \
    void vector_push_last_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem)    \
    {                                                                                 \
        if(ctx->length == ctx->capacity)                                              \
        {                                                                             \
            vector_grow_##SUFFIX(ctx);                                                \
        }                                                                             \
                                                                                      \
        ctx->array[ctx->length] = elem;                                               \
        ctx->length++;                                                                \
    }

#define VECTOR_PUSH_FIRST(TYPE, SUFFIX)                                               \
    void vector_push_first_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem)   \
    {                                                                                 \
        vector_insert_##SUFFIX(ctx, elem, 0);                                         \
    }                                                                                 \

#define VECTOR_POP_LAST(TYPE, SUFFIX)                                                              \
    enum vector_status vector_pop_last_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE* elem)   \
    {                                                                                              \
        if(ctx->length == 0)                                                                       \
        {                                                                                          \
            return EMPTY;                                                                          \
        }                                                                                          \
                                                                                                   \
        *elem = ctx->array[ctx->length-1];                                                         \
        ctx->length--;                                                                             \
        return OK;                                                                                 \
    }

#define VECTOR_POP_FIRST(TYPE, SUFFIX)                                                              \
    enum vector_status vector_pop_first_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE* elem)   \
    {                                                                                               \
        return vector_remove_##SUFFIX(ctx, 0, elem);                                                \
    }                                                                                               \

#define VECTOR_REMOVE(TYPE, SUFFIX)                                                                         \
    enum vector_status vector_remove_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx, TYPE* elem)     \
    {                                                                                                       \
        if(idx < 0 || idx >= ctx->length)                                                                   \
        {                                                                                                   \
            return OUT_OF_BOUNDS;                                                                           \
        }                                                                                                   \
                                                                                                            \
        *elem = ctx->array[idx];                                                                            \
        vector_move_left_##SUFFIX(ctx, idx);                                                                \
        ctx->length--;                                                                                      \
        return OK;                                                                                          \
    }                                                                                                       \


#define VECTOR_INSERT(TYPE, SUFFIX)                                                                     \
    enum vector_status vector_insert_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem, int idx)  \
    {                                                                                                   \
        if(idx < 0 || idx > ctx->length)                                                                \
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


#define VECTOR_INDEX(TYPE, SUFFIX)                                                                          \
                                                                                                                                               \
    enum vector_status vector_index_of4_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem, int* idx, int (*equals)(TYPE lhs, TYPE rhs))  \
    {                                                                                                                                          \
        if(equals == NULL)                                                                                                                     \
        {                                                                                                                                      \
            equals = &vector_equals_##SUFFIX;                                                                                                  \
        }                                                                                                                                      \
                                                                                                                                               \
        for(int i=0;i<ctx->length;i++)                                                                                                         \
        {                                                                                                                                      \
            if((*equals)(ctx->array[i], elem))                                                                                                 \
            {                                                                                                                                  \
                *idx = i;                                                                                                                      \
                return OK;                                                                                                                     \
            }                                                                                                                                  \
        }                                                                                                                                      \
                                                                                                                                               \
        return NOT_FOUND;                                                                                                                      \
    };                                                                                                                                         \
                                                                                                                                               \
    enum vector_status vector_index_of3_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem, int* idx)  \
    {                                                                                                       \
        return vector_index_of4_##SUFFIX(ctx, elem, idx, NULL);                                             \
    };                                                                                                      \
                                                                                                            \

#define VECTOR_REDUCE(TYPE, SUFFIX)                                                                                     \
    TYPE vector_reduce_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE acc, TYPE (*reduce_op)(TYPE lhs, TYPE rhs))   \
    {                                                                                                                   \
        for(int i=0;i<ctx->length;i++)                                                                                  \
        {                                                                                                               \
            acc = (*reduce_op)(acc, ctx->array[i]);                                                                     \
        }                                                                                                               \
                                                                                                                        \
        return acc;                                                                                                     \
    }                                                                                                                   \

#define VECTOR_FILTER(TYPE, SUFFIX)                                                             \
    void vector_filter_##SUFFIX(struct vector_context_##SUFFIX* ctx, int (*pred)(TYPE elem))    \
    {                                                                                           \
        for(int i=ctx->length-1;i>=0;i--)                                                       \
        {                                                                                       \
            if(!(*pred)(ctx->array[i]))                                                         \
            {                                                                                   \
                TYPE elem;                                                                      \
                vector_remove_##SUFFIX(ctx, i, &elem);                                          \
            }                                                                                   \
        }                                                                                       \
    }                                                                                           \

#define VECTOR_MAP(TYPE, SUFFIX)                                                                \
    void vector_map_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE (*map_op)(TYPE elem))    \
    {                                                                                           \
        for(int i=0;i<ctx->length;i++)                                                          \
        {                                                                                       \
            ctx->array[i] = (*map_op)(ctx->array[i]);                                           \
        }                                                                                       \
    }                                                                                           \

#define VECTOR_CLONE(TYPE, SUFFIX)                                                              \
    struct vector_context_##SUFFIX vector_clone_##SUFFIX(struct vector_context_##SUFFIX* ctx)   \
    {                                                                                           \
        struct vector_context_##SUFFIX newCtx = {                                               \
            .array = (TYPE *)malloc(sizeof(TYPE) * ctx->capacity),                              \
            .capacity = ctx->capacity,                                                          \
            .length = ctx->length                                                               \
        };                                                                                      \
                                                                                                \
        for(int i=0;i<ctx->length;i++)                                                          \
        {                                                                                       \
            newCtx.array[i] = ctx->array[i];                                                    \
        }                                                                                       \
                                                                                                \
        return newCtx;                                                                          \
    }                                                                                           \

#define VECTOR_GET(TYPE, SUFFIX)                                                                     \
    enum vector_status vector_get_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx, TYPE* elem) \
    {                                                                                                \
        if(idx < 0 || idx >= ctx->length)                                                            \
        {                                                                                            \
            return OUT_OF_BOUNDS;                                                                    \
        }                                                                                            \
                                                                                                     \
        *elem = ctx->array[idx];                                                                     \
        return OK;                                                                                   \
    }                                                                                                \

#define VECTOR_SET(TYPE, SUFFIX)                                                                     \
    enum vector_status vector_set_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx, TYPE elem)  \
    {                                                                                                \
        if(idx < 0 || idx >= ctx->length)                                                            \
        {                                                                                            \
            return OUT_OF_BOUNDS;                                                                    \
        }                                                                                            \
                                                                                                     \
        ctx->array[idx] = elem;                                                                      \
        return OK;                                                                                   \
    }  


char* string_concat(char* lhs, char* rhs)
{
    int len_lhs = strlen(lhs);
    int len_rhs = strlen(rhs);

    char* new_str = (char*) malloc((len_lhs+len_rhs+1)*sizeof(char));
    strcpy(new_str, lhs); 
    new_str = strcat(new_str, rhs); 

    return new_str;
}


#define VECTOR_TO_STRING(TYPE, SUFFIX)                                                               \
    char* vector_to_string_##SUFFIX(struct vector_context_##SUFFIX* ctx, char* (*fmt)(TYPE el))      \
    {                                                                                                \
        char* buf = "[";                                                                             \
        int i=0;                                                                                     \
        for(;i<ctx->length-1;i++)                                                                    \
        {                                                                                            \
            buf = string_concat(buf, (*fmt)(ctx->array[i]));                                         \
            buf = string_concat(buf, ", ");                                                          \
        }                                                                                            \
        if(i<ctx->length)                                                                            \
        {                                                                                            \
            buf = string_concat(buf, (*fmt)(ctx->array[i]));                                         \
        }                                                                                            \
                                                                                                     \
        return string_concat(buf, "]");                                                              \
    }                                                                                                \


#define VECTOR_DEFINE_ALL(TYPE, SUFFIX)                                  \
    VECTOR_CONTEXT(TYPE, SUFFIX)                                         \
    VECTOR_DEFAULT_EQUALS(TYPE, SUFFIX)                                  \
    VECTOR_INIT(TYPE, SUFFIX)                                            \
    VECTOR_DESTROY(TYPE, SUFFIX)                                         \
    VECTOR_GROW(TYPE, SUFFIX)                                            \
    VECTOR_MOVE_RIGHT(TYPE, SUFFIX)                                      \
    VECTOR_MOVE_LEFT(TYPE, SUFFIX)                                       \
    VECTOR_REMOVE(TYPE, SUFFIX)                                          \
    VECTOR_INSERT(TYPE, SUFFIX)                                          \
    VECTOR_PUSH_LAST(TYPE, SUFFIX)                                       \
    VECTOR_PUSH_FIRST(TYPE, SUFFIX)                                      \
    VECTOR_POP_LAST(TYPE, SUFFIX)                                        \
    VECTOR_POP_FIRST(TYPE, SUFFIX)                                       \
    VECTOR_INDEX(TYPE, SUFFIX)                                           \
    VECTOR_REDUCE(TYPE, SUFFIX)                                          \
    VECTOR_FILTER(TYPE, SUFFIX)                                          \
    VECTOR_MAP(TYPE, SUFFIX)                                             \
    VECTOR_CLONE(TYPE, SUFFIX)                                           \
    VECTOR_GET(TYPE, SUFFIX)                                             \
    VECTOR_SET(TYPE, SUFFIX)                                             \
    VECTOR_TO_STRING(TYPE, SUFFIX)                                       \
    VECTOR_ASSERT(TYPE, SUFFIX)                                          \

#define FOREACH(CTX, TYPE, IT) int _x=0; TYPE IT=CTX.array[_x];  for(;_x<CTX.length;IT=CTX.array[++_x])