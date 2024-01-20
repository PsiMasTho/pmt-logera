#include "../../lib/algo.h"

#include <assert.h>
#include <string.h>

static bool int_is_even(const void* ptr)
{
    int value = *(int*)ptr;
    return value % 2 == 0;
}

static int int_cmp(const void* lhs, const void* rhs)
{
    return memcmp(lhs, rhs, sizeof(int));
}

static void test_swap(void)
{
    int a = 42;
    int b = 1337;
    swap(&a, &b, sizeof(int));
    assert(a == 1337);
    assert(b == 42);
}

static void test_swap_same_ptr(void)
{
    int a = 42;
    swap(&a, &a, sizeof(int));
    assert(a == 42);
}

static void test_distance(void)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int* begin = arr;
    int* end   = arr + sizeof(arr) / sizeof(int);

    assert(distance(begin, end, sizeof(int)) == 5);
    assert(distance(end, begin, sizeof(int)) == -5);
}

static void test_between(void)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int* begin = arr;
    int* end   = arr + sizeof(arr) / sizeof(int);

    assert(between(begin, end, begin));
    assert(between(begin, end, begin + 1));
    assert(between(begin, end, begin + 2));
    assert(between(begin, end, begin + 3));
    assert(between(begin, end, begin + 4));
    assert(!between(begin, end, begin - 1));
    assert(!between(begin, end, begin + 5));
}

static void test_find_val(void)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int* begin = arr;
    int* end   = arr + sizeof(arr) / sizeof(int);

    int value = 3;
    int* it = find_val(begin, end, &value, sizeof(int));
    assert(it == begin + 2);
}

static void test_find_not_val(void)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int* begin = arr;
    int* end   = arr + sizeof(arr) / sizeof(int);

    int value = 1;
    int* it = find_not_val(begin, end, &value, sizeof(int));
    assert(it == begin + 1);
}

static void test_find_if(void)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    int* begin = arr;
    int* end   = arr + sizeof(arr) / sizeof(int);

    int* it = find_if(begin, end, sizeof(int), int_is_even);
    assert(it == begin + 1);
}

static void test_reverse(void)
{
    int arr_initial[]  = { 1, 2, 3, 4, 5 };
    int arr_expected[] = { 5, 4, 3, 2, 1 };

    int* begin = arr_initial;
    int* end   = arr_initial + sizeof(arr_initial) / sizeof(int);

    reverse(begin, end, sizeof(int));

    assert(memcmp(arr_initial, arr_expected, sizeof(arr_expected)) == 0);
}

static void test_reverse_empty(void)
{
    opaque_vector empty = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);

    reverse(empty.begin, empty.end, sizeof(int));

    opaque_vector_destroy(&empty);
}

static void test_duplicates()
{
    int arr[]             = { 1, 2, 2, 3, 4, 4, 4, 5, 5, 5 };
    int expected_dup[]    = {    2, 2,    4, 4, 4, 5, 5, 5 };
    int* begin = arr;
    int* end   = arr + sizeof(arr) / sizeof(int);

    opaque_vector dup = duplicates(begin, end, sizeof(int), int_cmp);
    
    assert(opaque_vector_size(&dup) == 8);

    for (int i = 0; i < opaque_vector_size(&dup); ++i)
        assert(**(int**)opaque_vector_at(&dup, i) == expected_dup[i]);

    opaque_vector_destroy(&dup);
}

static void test_excess_duplicates(void)
{
    int arr[]             = { 1, 2, 2, 3, 4, 4, 4, 5, 5, 5 };
    int expected_excess[] = {       2,       4, 4,    5, 5 };
    int* begin = arr;
    int* end   = arr + sizeof(arr) / sizeof(int);

    opaque_vector excess = excess_duplicates(begin, end, sizeof(int), int_cmp);
    
    assert(opaque_vector_size(&excess) == 5);

    for (int i = 0; i < opaque_vector_size(&excess); ++i)
        assert(**(int**)opaque_vector_at(&excess, i) == expected_excess[i]);

    opaque_vector_destroy(&excess);
}

static void test_unique(void)
{
    int arr[]             = { 1, 2, 2, 3, 4, 4, 4, 5, 5, 5 };
    //int expected[]      = { 1, 2,    3, 4,       5 };
    int expected_indices[]  = { 0, 1, 3, 4, 7 };
    
    opaque_vector unique_vals = unique(arr, arr + sizeof(arr) / sizeof(int), sizeof(int), int_cmp);

    assert(opaque_vector_size(&unique_vals) == 5);

    // check that the unique ptrs point to the first occurence of the value
    for (int i = 0; i < opaque_vector_size(&unique_vals); ++i)
        assert(*(int**)opaque_vector_at(&unique_vals, i) == arr + expected_indices[i]);
    
    opaque_vector_destroy(&unique_vals);
}

void test_algo(void)
{
    test_swap();
    test_swap_same_ptr();
    test_distance();
    test_between();
    test_find_val();
    test_find_not_val();
    test_find_if();
    test_reverse();
    test_reverse_empty();
    test_duplicates();
    test_excess_duplicates();
    test_unique();
}
