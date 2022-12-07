#include <stdio.h>
#include <stdlib.h>
#include "../include/vector.h"

VECTOR_DEFINE_ALL(double, d)

char* fmt_op(double el)
{
    int length = snprintf( NULL, 0, "%f", el );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%f", el );
    return str;
}

int main (void) 
{
    struct vector_context_d ctx =  vector_init0_d();

    vector_push_first_d(&ctx, 1.0);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    vector_push_first_d(&ctx, 2.0);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    vector_push_first_d(&ctx, 3.0);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    vector_push_first_d(&ctx, 4.0);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    double pop;
    vector_pop_first_d(&ctx, &pop);
    printf("Popped first %f\n", pop);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    vector_push_last_d(&ctx, 5.0);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    vector_push_last_d(&ctx, 6.0);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    vector_push_last_d(&ctx, 7.0);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    vector_pop_last_d(&ctx, &pop);
    printf("Popped last %f\n", pop);

    vector_set_d(&ctx, 2, 100.0);
    printf("Setting [%d] to %f\n", 2, 100.0);
    printf("Vector: %s\n", vector_to_string_d(&ctx, &fmt_op));

    puts("Vector contents: \n");
    FOREACH(ctx, double, it)
    {
        printf("%f ", it);
    }
    puts("\n");

    return 0;
}