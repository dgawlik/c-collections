#include <stdlib.h>
#include <check.h>
#include "../include/vector.h"
 

VECTOR_DEFINE_ALL(int, int);

START_TEST(test_allocation)
{
    struct vector_context_int ctx = vector_init1_int(10);
    ck_assert_int_eq(ctx.capacity, 10);
    vector_destroy_int(&ctx);
}
END_TEST
 
START_TEST(test_check_bounds)
{
    struct vector_context_int ctx = vector_init1_int(10);
    ck_assert(!is_inside_bounds_int(&ctx, 1));
    ck_assert(!is_inside_bounds_int(&ctx, -1));
    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_add)
{
    struct vector_context_int ctx = vector_init1_int(10);
    vector_push_int(&ctx, 1);
    vector_push_int(&ctx, 2);
    vector_push_int(&ctx, 3);
    ck_assert(is_vector_equal(&ctx, 3, 1, 2 ,3));
    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_grow)
{
    struct vector_context_int ctx = vector_init1_int(2);
    vector_push_int(&ctx, 1);
    vector_push_int(&ctx, 2);
    vector_push_int(&ctx, 3);
    ck_assert(is_vector_equal(&ctx, 3, 1, 2 ,3));
    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_insert)
{
    struct vector_context_int ctx = vector_init1_int(2);
    vector_push_int(&ctx, 1);
    vector_insert_int(&ctx, 2, 0);
    vector_insert_int(&ctx, 3, 0);
    ck_assert(is_vector_equal(&ctx, 3, 3, 2, 1));
    ck_assert(vector_insert_int(&ctx, 4, 5) == OUT_OF_BOUNDS);
    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_pop)
{
    struct vector_context_int ctx = vector_init1_int(2);
    vector_push_int(&ctx, 1);
    vector_insert_int(&ctx, 2, 0);
    vector_insert_int(&ctx, 3, 0);

    int pop1;
    vector_pop_int(&ctx, &pop1);
    ck_assert_int_eq(pop1, 1);

    int pop2;
    vector_pop_int(&ctx, &pop2);
    ck_assert_int_eq(pop2, 2);

     int pop3;
    vector_pop_int(&ctx, &pop3);
    ck_assert_int_eq(pop3, 3);

    int pop4;
    ck_assert(vector_pop_int(&ctx, &pop4) == EMPTY);

    vector_destroy_int(&ctx);
}
END_TEST


#define TEST_ADD(label, var, test)      \
    TCase* var = tcase_create(label);   \
    tcase_add_test(var, test);          \
    suite_add_tcase(s, var);


Suite * tests(void)
{
    Suite *s =  suite_create("vector");

    TEST_ADD("Core", tc_core, test_allocation);
    TEST_ADD("Bounds", tc_bounds, test_check_bounds);
    TEST_ADD("Push", tc_push, test_add);
    TEST_ADD("Grow", tc_grow, test_grow);
    TEST_ADD("Insert", tc_insert, test_insert);
    TEST_ADD("Pop", tc_pop, test_pop);
    
    return s;
}

 int main(void)
 {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = tests();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }