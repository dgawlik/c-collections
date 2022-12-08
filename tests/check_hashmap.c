#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <string.h>
#include "../include/hashmap.h"
 


HASHMAP_DEFINE_ALL(int, int, i);


int equals(int l, int r)
{
    return l == r;
}

int hash_code(int k)
{
    return k;
}


START_TEST(test_init)
{
    struct hashmap_context_i ctx = hashmap_init_i(&hash_code, &equals);
}
END_TEST


#define TEST_ADD(label, var, test)      \
    TCase* var = tcase_create(label);   \
    tcase_add_test(var, test);          \
    suite_add_tcase(s, var);


Suite * tests(void)
{
    Suite *s =  suite_create("hashmap");

    TEST_ADD("Init", tc_init, test_init);

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