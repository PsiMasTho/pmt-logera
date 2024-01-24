#include "algo.h"

#include <assert.h>
#include <string.h>

#define MAX_SWAP_STRIDE 1024
#define VOID_PTR_FROM(x) ((void*)(x))

void* advance(void const* const p, int const n, int const stride)
{
    return (char*)p + n * stride;
}

bool between(void const* const first, void const* const last, void const* const target)
{
    assert(first != NULL && last != NULL && target != NULL);
    assert(first <= last);

    return first <= target && target < last;
}

int distance(void const* const first, void const* const last, int const stride)
{
    assert(stride > 0);
    assert((last - first) % stride == 0);

    return (last - first) / stride;
}

opaque_vector duplicates(void const* first, void const* const last, int const stride, int (*const cmp)(void const*, void const*))
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(stride > 0);
    assert(cmp != NULL);

    opaque_vector ret = opaque_vector_create(sizeof(void*), opaque_vector_destroy_trivial);

    while (first != last)
    {
        void* const next = find_not_cmp(first, last, stride, first, cmp);

        if (distance(first, next, stride) > 1)
            for (void const* i = first; i != next; i = advance(i, 1, stride))
                opaque_vector_push(&ret, &i);

        first = next;
    }

    return ret;
}

opaque_vector excess_duplicates(void const* first, void const* const last, int const stride, int (*const cmp)(void const*, void const*))
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(stride > 0);
    assert(cmp != NULL);

    opaque_vector ret = opaque_vector_create(sizeof(void*), opaque_vector_destroy_trivial);

    while (first != last)
    {
        void* lookahead = advance(first, 1, stride);
        for (; lookahead != last; lookahead = advance(lookahead, 1, stride))
        {
            if (cmp(first, lookahead) != 0)
                break;
            opaque_vector_push(&ret, &lookahead);
        }
        first = lookahead;
    }

    return ret;
}

void* find_cmp(
    void const* first,
    void const* const last,
    int const stride,
    void const* const value,
    int (*const cmp)(void const*, void const*))
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(stride > 0);
    assert(value != NULL);
    assert(cmp != NULL);

    for (; first != last; first = advance(first, 1, stride))
        if (cmp(first, value) == 0)
            return VOID_PTR_FROM(first);
    return VOID_PTR_FROM(last);
}

void* find_if_not(void const* first, void const* const last, int const stride, bool (*const pred)(void const*))
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(stride > 0);
    assert(pred != NULL);

    for (; first != last; first = advance(first, 1, stride))
        if (!pred(first))
            return VOID_PTR_FROM(first);
    return VOID_PTR_FROM(last);
}

void* find_if(void const* first, void const* const last, int const stride, bool (*const pred)(void const*))
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(stride > 0);
    assert(pred != NULL);

    for (; first != last; first = advance(first, 1, stride))
        if (pred(first))
            return VOID_PTR_FROM(first);
    return VOID_PTR_FROM(last);
}

void* find_not_cmp(
    void const* first,
    void const* const last,
    int const stride,
    void const* const value,
    int (*const cmp)(void const*, void const*))
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(stride > 0);
    assert(value != NULL);
    assert(cmp != NULL);

    for (; first != last; first = advance(first, 1, stride))
        if (cmp(first, value) != 0)
            return VOID_PTR_FROM(first);
    return VOID_PTR_FROM(last);
}

void* find_not_val(void const* first, void const* const last, void const* const value, int const stride)
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(value != NULL);
    assert(stride > 0);

    for (; first != last; first = advance(first, 1, stride))
        if (memcmp(first, value, stride) != 0)
            return VOID_PTR_FROM(first);
    return VOID_PTR_FROM(last);
}

void* find_val(void const* first, void const* const last, void const* const value, int const stride)
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(value != NULL);
    assert(stride > 0);

    for (; first != last; first = advance(first, 1, stride))
        if (memcmp(first, value, stride) == 0)
            return VOID_PTR_FROM(first);
    return VOID_PTR_FROM(last);
}

void reverse(void* first, void* last, int const stride)
{
    assert((first == last) || (first != NULL && last != NULL));
    assert(first <= last);
    assert(stride > 0);

    while (first != last && first != advance(last, -1, stride))
    {
        swap(first, advance(last, -1, stride), stride);
        first = advance(first, 1, stride);
        last = advance(last, -1, stride);
    }
}

void swap(void* const lhs, void* const rhs, int const stride)
{
    assert(lhs != NULL && rhs != NULL);
    assert(stride > 0);
    assert(stride <= MAX_SWAP_STRIDE);

    char tmp[stride]; // VLA
    memcpy(tmp, lhs, stride);
    memcpy(lhs, rhs, stride);
    memcpy(rhs, tmp, stride);
}

opaque_vector unique(void const* first, void const* last, int const stride, int (*const cmp)(void const*, void const*))
{
    assert(first != NULL);
    assert(last != NULL);
    assert(stride > 0);
    assert(cmp != NULL);

    opaque_vector ret = opaque_vector_create(sizeof(void*), opaque_vector_destroy_trivial);

    for (; first != last; first = find_not_cmp(first, last, stride, first, cmp))
        opaque_vector_push(&ret, &first);

    return ret;
}

void* upper_bound(void const* first, void const* last, int stride, void const* value, int (*cmp)(void const*, void const*))
{
    void const* l = first;
    void const* h = last;
    while (l < h)
    {
        void const* mid = advance(l, distance(l, h, stride) / 2, stride);
        if (cmp(value, mid) >= 0)
            l = advance(mid, 1, stride);
        else
            h = mid;
    }
    return VOID_PTR_FROM(l);
}

void* lower_bound(void const* first, void const* last, int stride, void const* value, int (*cmp)(void const*, void const*))
{
    void const* l = first;
    void const* h = last;
    while (l < h)
    {
        void const* mid = advance(l, distance(l, h, stride) / 2, stride);
        if (cmp(value, mid) <= 0)
            h = mid;
        else
            l = advance(mid, 1, stride);
    }
    return VOID_PTR_FROM(l);
}
