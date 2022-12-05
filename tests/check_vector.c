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

Suite * tests(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_bounds;
    TCase *tc_add;
    TCase *tc_grow;

    s = suite_create("vector");
    
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_allocation);
    suite_add_tcase(s, tc_core);

    tc_bounds = tcase_create("Bounds");
    tcase_add_test(tc_bounds, test_check_bounds);
    suite_add_tcase(s, tc_bounds);

    tc_add = tcase_create("Add");
    tcase_add_test(tc_add, test_add);
    suite_add_tcase(s, tc_add);

    tc_grow = tcase_create("Grow");
    tcase_add_test(tc_grow, test_add);
    suite_add_tcase(s, tc_grow);

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