#include "../../lib/opaque_vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int num;
    char* str;
} ov_test_struct; // opaque vector test struct

static char s_sample_str_1[] = "this is a sample string";
static char s_sample_str_2[] = "this is another sample string";

static void ov_test_struct_destroy(void* self)
{
    assert(self != NULL);
    ov_test_struct* const s = (ov_test_struct*)self;
    free(s->str);
    s->str = NULL;
}

static bool int_is_even(const void* ptr)
{
    int value = *(int*)ptr;
    return value % 2 == 0;
}

static bool int_is_prime(const void* ptr)
{
    int value = *(int*)ptr;
    if (value < 2)
        return false;
    for (int i = 2; i < value; ++i)
        if (value % i == 0)
            return false;
    return true;
}

static bool pred_always_true(const void* ptr)
{
    return true;
}

static void test_opaque_vector_create(void)
{
    opaque_vector v = opaque_vector_create(sizeof(ov_test_struct), ov_test_struct_destroy);
    assert(opaque_vector_size(&v) == 0);
    assert(opaque_vector_capacity(&v) == 0);
    opaque_vector_destroy(&v);
}

static void test_opaque_vector_move(void)
{
    opaque_vector v = opaque_vector_create(sizeof(ov_test_struct), ov_test_struct_destroy);
    ov_test_struct s = { .num = 42, .str = NULL };
    opaque_vector_push(&v, &s);
    opaque_vector v2 = opaque_vector_move(&v);
    
    assert(opaque_vector_size(&v) == 0);
    assert(opaque_vector_capacity(&v) == 0);

    assert(opaque_vector_size(&v2) == 1);
    assert(opaque_vector_capacity(&v2) > 0);
    
    opaque_vector_destroy(&v);
    opaque_vector_destroy(&v2);
}

static void test_opaque_vector_insert_one(void)
{
    opaque_vector vi = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    int n = 42;
    opaque_vector_insert_one(&vi, vi.end, &n);

    assert(opaque_vector_size(&vi) == 1);
    assert(opaque_vector_capacity(&vi) > 0);

    int* const n2 = opaque_vector_at(&vi, 0);
    assert(*n2 == 42);

    opaque_vector_destroy(&vi);
}

static void test_opaque_vector_insert_many(void)
{
    opaque_vector vi = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    for (int i = 0; i < 100; ++i)
        opaque_vector_push(&vi, &i);
    
    int const to_insert[] = { 666, 777, 888, 999 };
    size_t const to_insert_size = sizeof(to_insert) / sizeof(to_insert[0]);

    opaque_vector_insert_many(&vi, (int*)vi.begin + 10, to_insert, to_insert + to_insert_size);

    assert(opaque_vector_size(&vi) == 100 + to_insert_size);

    for (int i = 0; i < 10; ++i)
    {
        int* const n = opaque_vector_at(&vi, i);
        assert(*n == i);
    }

    for (int i = 0; i < to_insert_size; ++i)
    {
        int* const n = opaque_vector_at(&vi, 10 + i);
        assert(*n == to_insert[i]);
    }

    for (int i = 0; i < 90; ++i)
    {
        int* const n = opaque_vector_at(&vi, 10 + to_insert_size + i);
        assert(*n == i + 10);
    }

    opaque_vector_destroy(&vi);
}

static void test_opaque_vector_push_one(void)
{
    opaque_vector v = opaque_vector_create(sizeof(ov_test_struct), ov_test_struct_destroy);
    ov_test_struct s = { .num = 42, .str = NULL };
    opaque_vector_push(&v, &s);
    
    assert(opaque_vector_size(&v) == 1);
    assert(opaque_vector_capacity(&v) > 0);

    ov_test_struct* const s2 = opaque_vector_at(&v, 0);
    assert(s2->num == 42);
    assert(s2->str == NULL);
    opaque_vector_destroy(&v);
}

static void test_opaque_vector_push_many(void)
{
    opaque_vector v = opaque_vector_create(sizeof(ov_test_struct), ov_test_struct_destroy);
    for (int i = 0; i < 100; ++i)
    {

        char* sample_str = malloc(sizeof(s_sample_str_1));
        strncpy(sample_str, s_sample_str_1, sizeof(s_sample_str_1));
        ov_test_struct s = { .num = i, .str = sample_str };
        opaque_vector_push(&v, &s);
    }

    assert(opaque_vector_size(&v) == 100);
    assert(opaque_vector_capacity(&v) >= 100);

    for (int i = 0; i < 100; ++i)
    {
        ov_test_struct* const s = opaque_vector_at(&v, i);
        assert(s->num == i);
        assert(strcmp(s->str, s_sample_str_1) == 0);
    }
    opaque_vector_destroy(&v);
}

static void test_opaque_vector_pop(void)
{
    opaque_vector v = opaque_vector_create(sizeof(ov_test_struct), ov_test_struct_destroy);
    ov_test_struct s = { .num = 42, .str = NULL };
    opaque_vector_push(&v, &s);
    opaque_vector_pop(&v);
    
    assert(opaque_vector_size(&v) == 0);
    assert(opaque_vector_capacity(&v) > 0);
    
    opaque_vector_destroy(&v);
}

static void test_opaque_vector_push_pop_alternate(void)
{
    opaque_vector v = opaque_vector_create(sizeof(ov_test_struct), ov_test_struct_destroy);
    for (int i = 0; i < 100; ++i)
    {
        char* sample_str = malloc(sizeof(s_sample_str_1));
        strncpy(sample_str, s_sample_str_1, sizeof(s_sample_str_1));
        ov_test_struct s = { .num = i, .str = sample_str };
        opaque_vector_push(&v, &s);
    }

    for (int i = 0; i < 100; ++i)
    {
        ov_test_struct* const s = opaque_vector_at(&v, i);
        assert(s->num == i);
        assert(strcmp(s->str, s_sample_str_1) == 0);
    }

    for (int i = 0; i < 100; ++i)
        opaque_vector_pop(&v);

    for (int i = 0; i < 100; ++i)
    {
        char* sample_str = malloc(sizeof(s_sample_str_2));
        strncpy(sample_str, s_sample_str_2, sizeof(s_sample_str_2));
        ov_test_struct s = { .num = i, .str = sample_str };
        opaque_vector_push(&v, &s);
    }

    for (int i = 0; i < 100; ++i)
    {
        ov_test_struct* const s = opaque_vector_at(&v, i);
        assert(s->num == i);
        assert(strcmp(s->str, s_sample_str_2) == 0);
    }

    opaque_vector_destroy(&v);
}

static void test_opaque_vector_reserve(void)
{
    opaque_vector v = opaque_vector_create(sizeof(ov_test_struct), ov_test_struct_destroy);
    opaque_vector_reserve(&v, 100);
    
    assert(opaque_vector_size(&v) == 0);
    assert(opaque_vector_capacity(&v) == 100);

    opaque_vector_destroy(&v);
}

static void test_opaque_vector_clear(void)
{
    opaque_vector v = opaque_vector_create(sizeof(ov_test_struct), ov_test_struct_destroy);
    for (int i = 0; i < 100; ++i)
    {
        char* sample_str = malloc(sizeof(s_sample_str_1));
        strncpy(sample_str, s_sample_str_1, sizeof(s_sample_str_1));
        ov_test_struct s = { .num = i, .str = sample_str };
        opaque_vector_push(&v, &s);
    }
    opaque_vector_clear(&v);
    
    assert(opaque_vector_size(&v) == 0);
    assert(opaque_vector_capacity(&v) > 0);

    opaque_vector_destroy(&v);
}

static void test_opaque_vector_erase_one(void)
{
    opaque_vector v = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    for (int i = 0; i < 100; ++i)
        opaque_vector_push(&v, &i);
    
    for (int i = 100; i > 0; --i)
    {
        int* const n = opaque_vector_at(&v, i - 1);
        if (i % 2 == 0)
            opaque_vector_erase_one(&v, n);
    }

    assert(opaque_vector_size(&v) == 50);
    assert(opaque_vector_capacity(&v) > 0);

    for (int i = 0; i < 50; ++i)
    {
        int* const n = opaque_vector_at(&v, i);
        assert(*n == i * 2);
    }

    opaque_vector_destroy(&v);
}

static void test_opaque_vector_erase_single(void)
{
    opaque_vector v = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    opaque_vector_push(&v, &(int){42});

    int* const ret = opaque_vector_erase_one(&v, (int*)v.begin);

    assert(opaque_vector_size(&v) == 0);
    assert(opaque_vector_capacity(&v) > 0);
    assert(ret == v.end);

    opaque_vector_destroy(&v);
}

static void test_opaque_vector_erase_many(void)
{
    opaque_vector v = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    for (int i = 0; i < 100; ++i)
        opaque_vector_push(&v, &i);
    
    int* const ret = opaque_vector_erase_many(&v, (int*)v.begin + 10, (int*)v.begin + 20);

    assert(opaque_vector_size(&v) == 90);
    assert(opaque_vector_capacity(&v) > 0);
    assert(*ret == 20);

    opaque_vector_destroy(&v);
}

static void test_opaque_vector_erase_many_single(void)
{
    opaque_vector v = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    opaque_vector_push(&v, &(int){42});

    int* const ret = opaque_vector_erase_many(&v, (int*)v.begin, (int*)v.begin + 1);

    assert(opaque_vector_size(&v) == 0);
    assert(opaque_vector_capacity(&v) > 0);
    assert(ret == v.end);

    opaque_vector_destroy(&v);
}

static void test_opaque_vector_erase_if_1(void)
{
    int arr_initial[]  = { 1, 2, 3, 4, 5, 6 };
    int arr_expected[] = { 1, 3, 5 };

    opaque_vector ov_initial = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    for (size_t i = 0; i < sizeof(arr_initial) / sizeof(arr_initial[0]); ++i)
        opaque_vector_push(&ov_initial, arr_initial + i);
    
    opaque_vector_erase_if(&ov_initial, int_is_even);

    assert(opaque_vector_size(&ov_initial) == sizeof(arr_expected) / sizeof(arr_expected[0]));
    assert(memcmp(ov_initial.begin, arr_expected, sizeof(arr_expected)) == 0);

    opaque_vector_destroy(&ov_initial);
}

static void test_opaque_vector_erase_if_2(void)
{
    opaque_vector v_initial = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    
    for (int i = 0; i < 100; ++i)
        opaque_vector_push(&v_initial, &i);
    
    opaque_vector v_expected = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);

    for (int i = 0; i < 100; ++i)
        if (!int_is_prime(&i))
            opaque_vector_push(&v_expected, &i);
    
    opaque_vector_erase_if(&v_initial, int_is_prime);

    assert(opaque_vector_size(&v_initial) == opaque_vector_size(&v_expected));

    for (size_t i = 0; i < opaque_vector_size(&v_initial); ++i)
        assert(*(int*)opaque_vector_at(&v_initial, i) == *(int*)opaque_vector_at(&v_expected, i));

    opaque_vector_destroy(&v_initial);
    opaque_vector_destroy(&v_expected);
}

static void test_opaque_vector_erase_if_3(void)
{
    opaque_vector v_initial = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    
    for (int i = 0; i < 100; ++i)
        opaque_vector_push(&v_initial, &i);
    
    opaque_vector_erase_if(&v_initial, pred_always_true);

    assert(opaque_vector_size(&v_initial) == 0);

    opaque_vector_destroy(&v_initial);
}

static void test_opaque_vector_2d(void)
{
    opaque_vector v = opaque_vector_create(sizeof(opaque_vector), opaque_vector_destroy);

    for (int i = 0; i < 100; ++i)
    {
        opaque_vector v2 = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
        for (int j = 0; j < 10; ++j)
            opaque_vector_push(&v2, &j);
        opaque_vector_push(&v, &v2);
    }

    for (int i = 0; i < 100; ++i)
    {
        opaque_vector* const v2 = opaque_vector_at(&v, i);
        for (int j = 0; j < 10; ++j)
        {
            int* const n = opaque_vector_at(v2, j);
            assert(*n == j);
        }
    }

    opaque_vector_destroy(&v);
}

static void test_opaque_vector_begin_end()
{
    opaque_vector v = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    for (int i = 0; i < 100; ++i)
        opaque_vector_push(&v, &i);
    
    int i = 0;
    for (int* n = v.begin; n != v.end; ++n)
        assert(*n == i++);
    
    opaque_vector_destroy(&v);
}

static void test_opaque_vector_begin_end_empty()
{
    opaque_vector v = opaque_vector_create(sizeof(int), opaque_vector_destroy_trivial);
    assert(v.begin == v.end);
    opaque_vector_destroy(&v);
}

static void test_opaque_vector_c_str()
{
    opaque_vector v = opaque_vector_create(sizeof(char*), c_str_destroy);
    char* s = strdup("hello world");
    opaque_vector_push(&v, &s);
    opaque_vector_destroy(&v);
}

void test_opaque_vector(void)
{
    test_opaque_vector_create();
    test_opaque_vector_move();
    test_opaque_vector_insert_one();
    test_opaque_vector_insert_many();
    test_opaque_vector_push_one();
    test_opaque_vector_push_many();
    test_opaque_vector_pop();
    test_opaque_vector_push_pop_alternate();
    test_opaque_vector_reserve();
    test_opaque_vector_clear();
    test_opaque_vector_erase_one();
    test_opaque_vector_erase_single();
    test_opaque_vector_erase_many();
    test_opaque_vector_erase_many_single();
    test_opaque_vector_erase_if_1();
    test_opaque_vector_erase_if_2();
    test_opaque_vector_erase_if_3();
    test_opaque_vector_2d();
    test_opaque_vector_begin_end();
    test_opaque_vector_begin_end_empty();
    test_opaque_vector_c_str();
}
