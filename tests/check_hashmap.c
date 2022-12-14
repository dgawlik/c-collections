#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <string.h>
#include "../include/hashmap.h"
 


HASHMAP_DEFINE_ALL(int, int, i);


int equals(int l, int r)
{
    return l == r ? 1 : 0;
}

int hash_code(int k)
{
    return k;
}

void free_int()
{

}
char* int_fmt(int el)
{
    int length = snprintf( NULL, 0, "%d", el );
char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", el );
    return str;
}


START_TEST(test_init)
{
    struct hashmap_context_i ctx = hashmap_init_i(hash_code, equals);
    hashmap_destroy_i(&ctx, free_int, free_int);
}
END_TEST

START_TEST(test_put)
{
    struct hashmap_context_i ctx = hashmap_init3_i(hash_code, equals, 1);

    hashmap_put_i(&ctx, 0, 1);
    hashmap_put_i(&ctx, 1, 0);
    hashmap_put_i(&ctx, 4, 2);

    ck_assert_int_eq(ctx.capacity, 4);
    ck_assert_int_eq(ctx.count, 3);

    ck_assert_int_eq(ctx.buckets[0]->value, 1);
    ck_assert_int_eq(ctx.buckets[0]->next->value, 2);
    ck_assert_int_eq(ctx.buckets[1]->value, 0);
    hashmap_destroy_i(&ctx, free_int, free_int);
}
END_TEST

START_TEST(test_get)
{
    struct hashmap_context_i ctx = hashmap_init3_i(hash_code, equals, 1);

    hashmap_put_i(&ctx, 0, 1);
    hashmap_put_i(&ctx, 1, 2);
    hashmap_put_i(&ctx, 2, 3);

    int value;
    hashmap_get_i(&ctx, 0, &value);
    ck_assert_int_eq(1, value);

    hashmap_get_i(&ctx, 1, &value);
    ck_assert_int_eq(2, value);

    hashmap_get_i(&ctx, 2, &value);
    ck_assert_int_eq(3, value);

    ck_assert(hashmap_get_i(&ctx, 3, &value) == HASH_NOT_FOUND);

    hashmap_destroy_i(&ctx, free_int, free_int);

}
END_TEST

START_TEST(test_remove)
{
    struct hashmap_context_i ctx = hashmap_init3_i(hash_code, equals, 1);

    hashmap_put_i(&ctx, 0, 1);
    hashmap_put_i(&ctx, 1, 0);
    hashmap_put_i(&ctx, 4, 2);

    ck_assert(hashmap_remove_i(&ctx, 4) == HASH_OK);
    ck_assert(hashmap_remove_i(&ctx, 4) == HASH_NOT_FOUND);

    ck_assert(hashmap_remove_i(&ctx, 0) == HASH_OK);
    ck_assert(hashmap_remove_i(&ctx, 0) == HASH_NOT_FOUND);

    ck_assert(hashmap_remove_i(&ctx, 1) == HASH_OK);
    ck_assert(hashmap_remove_i(&ctx, 1) == HASH_NOT_FOUND);

    hashmap_destroy_i(&ctx, free_int, free_int);
}
END_TEST

START_TEST(test_contains_key)
{
    struct hashmap_context_i ctx = hashmap_init3_i(hash_code, equals, 1);
    hashmap_put_i(&ctx, 0, 1);

    ck_assert(hashmap_contains_key(&ctx, 0));
    ck_assert(!hashmap_contains_key(&ctx, 1));

    hashmap_destroy_i(&ctx, free_int, free_int);
}
END_TEST

START_TEST(test_keys)
{
    struct hashmap_context_i ctx = hashmap_init3_i(hash_code, equals, 1);
    hashmap_put_i(&ctx, 0, 1);
    hashmap_put_i(&ctx, 1, 0);
    hashmap_put_i(&ctx, 4, 2);   

    int len = hashmap_key_count_i(&ctx);
    ck_assert_int_eq(3, len);

    int* keys = hashmap_keys_i(&ctx);
    ck_assert_int_eq(0, keys[0]);
    ck_assert_int_eq(4, keys[1]);
    ck_assert_int_eq(1, keys[2]);

    hashmap_destroy_i(&ctx, free_int, free_int);
}
END_TEST

START_TEST(test_to_string)
{
    struct hashmap_context_i ctx = hashmap_init3_i(hash_code, equals, 1);
    hashmap_put_i(&ctx, 1, 2);
    hashmap_put_i(&ctx, 3, 4);
    hashmap_put_i(&ctx, 5, 6);   

    char* repr = hashmap_to_string_i(&ctx, int_fmt, int_fmt);

    ck_assert_pstr_eq("{1 : 2, 5 : 6, 3 : 4}", repr);

    hashmap_destroy_i(&ctx, free_int, free_int);
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
    TEST_ADD("Put", tc_put, test_put);
    TEST_ADD("Get", tc_get, test_get);
    TEST_ADD("Remove", tc_remove, test_remove);
    TEST_ADD("Contains", tc_contains, test_contains_key);
    TEST_ADD("Keys", tc_keys, test_keys);
    TEST_ADD("ToString", tc_to_string, test_to_string);

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