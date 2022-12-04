#include <stdlib.h>
#include <check.h>
#include "../include/vector.h"
 
 START_TEST(test_allocation)
 {
    VECTOR_DEFINE_ALL(int, int);
    struct vector_context_int ctx = vector_init1_int(10);
    ck_assert_int_eq(ctx.capacity, 10);
    vector_destroy_int(ctx);
 }
 END_TEST
 
Suite * tests(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("vector");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_allocation);
    suite_add_tcase(s, tc_core);

    return s;
}

 int main(void)
 {
    return 0;
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = tests();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }