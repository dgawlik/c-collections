#include <stdio.h>
#include <stdlib.h>
#include "../include/vector.h"

VECTOR_DEFINE_ALL(double, d)

int main (void) 
{
    struct vector_context_d ctx =  vector_init0_d();

    vector_push_first_d(&ctx, 1.0);
    vector_push_first_d(&ctx, 2.0);
    vector_push_first_d(&ctx, 3.0);
    vector_push_first_d(&ctx, 4.0);

    double pop;
    vector_pop_first_d(&ctx, &pop);

    vector_push_last_d(&ctx, 5.0);
    vector_push_last_d(&ctx, 6.0);
    vector_push_last_d(&ctx, 7.0);

    vector_pop_last_d(&ctx, &pop);

    puts("Vector contents: \n");
    FOREACH(ctx, double, it)
    {
        printf("%f ", it);
    }
    puts("\n");

    return 0;
}