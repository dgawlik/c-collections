#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <string.h>
#include "../include/vector.h"
 

VECTOR_DEFINE_ALL(int, int);

struct person {
    char* first_name;
    char* last_name;
};

VECTOR_DEFINE_ALL(struct person, person);

struct complex {
    double first;
    double second;
};

VECTOR_DEFINE_ALL(struct complex, complex);

START_TEST(test_allocation)
{
    struct vector_context_int ctx = vector_init1_int(10);
    ck_assert_int_eq(ctx.capacity, 10);
    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_add)
{
    struct vector_context_int ctx = vector_init1_int(10);
    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);
    ck_assert(is_vector_equal_int(&ctx, 3, 1, 2 ,3));
    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_grow)
{
    struct vector_context_int ctx = vector_init1_int(2);
    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);
    ck_assert(is_vector_equal_int(&ctx, 3, 1, 2 ,3));
    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_insert)
{
    struct vector_context_int ctx = vector_init1_int(2);
    vector_push_last_int(&ctx, 1);
    vector_insert_int(&ctx, 2, 0);
    vector_insert_int(&ctx, 3, 0);
    ck_assert(is_vector_equal_int(&ctx, 3, 3, 2, 1));
    ck_assert(vector_insert_int(&ctx, 4, 5) == OUT_OF_BOUNDS);
    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_pop)
{
    struct vector_context_int ctx = vector_init1_int(2);
    vector_push_last_int(&ctx, 1);
    vector_insert_int(&ctx, 2, 0);
    vector_insert_int(&ctx, 3, 0);

    int pop1;
    vector_pop_last_int(&ctx, &pop1);
    ck_assert_int_eq(pop1, 1);

    int pop2;
    vector_pop_last_int(&ctx, &pop2);
    ck_assert_int_eq(pop2, 2);

     int pop3;
    vector_pop_last_int(&ctx, &pop3);
    ck_assert_int_eq(pop3, 3);

    int pop4;
    ck_assert(vector_pop_last_int(&ctx, &pop4) == EMPTY);

    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_remove)
{
    struct vector_context_int ctx = vector_init1_int(2);
    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);


    int rm1;
    vector_remove_int(&ctx, 1, &rm1);
    ck_assert_int_eq(rm1, 2);

    int rm2;
    vector_remove_int(&ctx, 0, &rm2);
    ck_assert_int_eq(rm2, 1);

    int rm3;
    vector_remove_int(&ctx, 0, &rm3);
    ck_assert_int_eq(rm3, 3);

    vector_destroy_int(&ctx);
}
END_TEST

START_TEST(test_index_of)
{
    struct vector_context_int ctx = vector_init1_int(2);
    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);


    int idx1;
    vector_index_of3_int(&ctx, 2, &idx1);
    ck_assert_int_eq(idx1, 1);

    int idx2;
    vector_index_of3_int(&ctx, 3, &idx2);
    ck_assert_int_eq(idx2, 2);

    int idx3;
    ck_assert(vector_index_of3_int(&ctx, 5, &idx3) == NOT_FOUND);

    vector_destroy_int(&ctx);
}
END_TEST

int custom_equals(struct person lhs, struct person rhs)
{
    return strcmp(lhs.first_name, rhs.first_name) == 0;
}

START_TEST(test_index_of_custom_equals)
{
    struct vector_context_person ctx = vector_init1_person(2);

    struct person p1 = {
        .first_name = "Dominik",
        .last_name = "Gawlik"
    };
    vector_push_last_person(&ctx, p1);

    struct person p2 = {
        .first_name = "Damian",
        .last_name = "Gawlik"
    };

    vector_push_last_person(&ctx, p2);


    int idx1;
    vector_index_of4_person(&ctx, p2, &idx1, &custom_equals);
    ck_assert_int_eq(idx1, 1);

    int idx2;
    vector_index_of4_person(&ctx, p1, &idx2, &custom_equals);
    ck_assert_int_eq(idx2, 0);

    vector_destroy_person(&ctx);
}
END_TEST

struct complex sum_op(struct complex lhs, struct complex rhs)
{
    struct complex s = {
        .first = lhs.first + rhs.first,
        .second = lhs.second + rhs.second
    };

    return s;
}

START_TEST(test_reduce)
{
    struct vector_context_complex ctx = vector_init1_complex(2);

    struct complex c1 = {
        .first = 1.0,
        .second = 2.0
    };

    vector_push_last_complex(&ctx, c1);

    struct complex c2 = {
        .first = 2,
        .second = 1
    };

    vector_push_last_complex(&ctx, c2);

    struct complex c3 = {
        .first = 2,
        .second = 2
    };

    vector_push_last_complex(&ctx, c3);

    struct complex zero = {
        .first = 0.0,
        .second = 0.0
    };

    struct complex sum = vector_reduce_complex(&ctx, zero, &sum_op);

    ck_assert_int_eq(sum.first, 5);
    ck_assert_int_eq(sum.second, 5);

    vector_destroy_complex(&ctx);
}
END_TEST

int pred(int elem)
{
    return elem % 2 == 0;
};

START_TEST(test_filter)
{
    struct vector_context_int ctx = vector_init0_int();

    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);
    vector_push_last_int(&ctx, 4);

    vector_filter_int(&ctx, &pred);

    ck_assert(is_vector_equal_int(&ctx, 2, 2, 4));
}
END_TEST

int multiply_by_two(int elem)
{
    return elem * 2;
};

START_TEST(test_map)
{
    struct vector_context_int ctx = vector_init0_int();

    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);

    vector_map_int(&ctx, &multiply_by_two);

    ck_assert(is_vector_equal_int(&ctx, 3, 2, 4, 6));
}
END_TEST

START_TEST(test_clone)
{
    struct vector_context_int ctx = vector_init0_int();

    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);

    struct vector_context_int ctx2 = vector_clone_int(&ctx);
    ck_assert(is_vector_equal_int(&ctx, 3, 1, 2, 3));
    ck_assert(ctx.array != ctx2.array);
}
END_TEST

START_TEST(test_foreach)
{
    struct vector_context_int ctx = vector_init0_int();

    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);

    int sum=0;
    FOREACH(ctx, int,  it)
    {
        sum += it;
    }

    ck_assert(sum == 6);
}

START_TEST(test_get_set)
{
    struct vector_context_int ctx = vector_init0_int();

    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);

    int get;
    vector_get_int(&ctx, 0, &get);

    ck_assert_int_eq(1, get);

    vector_set_int(&ctx, 1, 4);

    int get2;
    vector_get_int(&ctx, 1, &get2);

    ck_assert_int_eq(4, get2);

    ck_assert(vector_get_int(&ctx, 2, &get2) == OUT_OF_BOUNDS);
}
END_TEST

char* fmt_op(int el)
{
    int length = snprintf( NULL, 0, "%d", el );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", el );
    return str;
}

START_TEST(test_to_string)
{
    struct vector_context_int ctx = vector_init0_int();

    vector_push_last_int(&ctx, 1);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 3);

    char* repr = vector_to_string_int(&ctx, &fmt_op);

    ck_assert_pstr_eq("[1, 2, 3]", repr);
}
END_TEST

int compare(int lhs, int rhs)
{
    return lhs - rhs;
}

START_TEST(test_sort)
{
    struct vector_context_int ctx = vector_init0_int();

    vector_push_last_int(&ctx, 5);
    vector_push_last_int(&ctx, 4);
    vector_push_last_int(&ctx, 3);
    vector_push_last_int(&ctx, 2);
    vector_push_last_int(&ctx, 1);

    vector_sort_int(&ctx, compare);

    char* repr = vector_to_string_int(&ctx, &fmt_op);

    ck_assert_pstr_eq("[1, 2, 3, 4, 5]", repr);
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
    TEST_ADD("Push", tc_push, test_add);
    TEST_ADD("Grow", tc_grow, test_grow);
    TEST_ADD("Insert", tc_insert, test_insert);
    TEST_ADD("Pop", tc_pop, test_pop);
    TEST_ADD("Remove", tc_remove, test_remove);
    TEST_ADD("IndexOf", tc_index_of, test_index_of);
    TEST_ADD("IndexOfStruct", tc_struct_index_of, test_index_of_custom_equals);
    TEST_ADD("Reduce", tc_reduce, test_reduce);
    TEST_ADD("Filter", tc_filter, test_filter);
    TEST_ADD("Map", tc_map, test_map);
    TEST_ADD("Clone", tc_clone, test_clone);
    TEST_ADD("Foreach", tc_foreach, test_foreach);
    TEST_ADD("GetSet", tc_get_set, test_get_set);
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