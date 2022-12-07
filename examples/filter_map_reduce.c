#include <stdlib.h>
#include <stdio.h>
#include "../include/vector.h"

VECTOR_DEFINE_ALL(int, i)

int greater_than_5(int i)
{
    return i > 5;
}

int multiply_by_2(int i)
{
    return i*2;
}

int sum(int lhs, int rhs)
{
    return lhs + rhs;
}

char* fmt_op(int el)
{
    int length = snprintf( NULL, 0, "%d", el );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", el );
    return str;
}

int main (void) 
{
    struct vector_context_i ctx = vector_init0_i();

    for(int i=0;i<10;i++)
    {
        vector_push_last_i(&ctx, i);
    }

    printf("Vector: %s\n", vector_to_string_i(&ctx, &fmt_op));

    vector_filter_i(&ctx, &greater_than_5);
    printf("Vector: %s\n", vector_to_string_i(&ctx, &fmt_op));

    vector_map_i(&ctx, &multiply_by_2);
    printf("Vector: %s\n", vector_to_string_i(&ctx, &fmt_op));

    int result = vector_reduce_i(&ctx, 0, &sum);
    printf("Sum: %d\n", result);

    return 0;
}