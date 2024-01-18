#include "algo.h"

#include <assert.h>
#include <string.h>

#define MAX_SWAP_STRIDE 1024

void swap(void* lhs, void* rhs, int stride)
{
    assert(lhs != NULL && rhs != NULL);
    assert(stride > 0);
    assert(stride <= MAX_SWAP_STRIDE);

    char tmp[stride]; // VLA
    memcpy(tmp, lhs, stride);
    memcpy(lhs, rhs, stride);
    memcpy(rhs, tmp, stride);
}

int distance(void const* first, void const* last, int stride)
{
    assert(stride > 0);
    assert((last - first) % stride == 0);

    return (last - first) / stride;
}

bool between(void const* first, void const* last, void const* target)
{
    assert(first != NULL && last != NULL && target != NULL);
    assert(first <= last);

    return first <= target && target < last;
}

void* advance(void const* it, int n, int stride)
{
    return (char*)it + n * stride; // silently casting away constness...
}

void* find_val(void* first, void* last, void const* value, int stride)
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(value != NULL);
    assert(stride > 0);

    char* it = first;
    while (it != last)
    {
        if (memcmp(it, value, stride) == 0)
            return it;
        it += stride;
    }
    return last;
}

void* find_not_val(void* first, void* last, void const* value, int stride)
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(value != NULL);
    assert(stride > 0);

    char* it = first;
    while (it != last)
    {
        if (memcmp(it, value, stride) != 0)
            return it;
        it += stride;
    }
    return last;
}

void* find_if(void* first, void* last, int stride, bool (*pred)(void const*))
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(stride > 0);
    assert(pred != NULL);

    char* it = first;
    while (it != last)
    {
        if (pred(it))
            return it;
        it += stride;
    }
    return last;
}

void* find_if_not(void* first, void* last, int stride, bool (*pred)(void const*))
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(stride > 0);
    assert(pred != NULL);

    char* it = first;
    while (it != last)
    {
        if (!pred(it))
            return it;
        it += stride;
    }
    return last;
}

void* find_cmp(void* first, void* last, int stride, void const* value, int (*cmp)(void const*, void const*))
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(stride > 0);
    assert(value != NULL);
    assert(cmp != NULL);

    char* it = first;
    while (it != last)
    {
        if (cmp(it, value) == 0)
            return it;
        it += stride;
    }
    return last;
}

void* find_not_cmp(void* first, void* last, int stride, void const* value, int (*cmp)(void const*, void const*))
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(stride > 0);
    assert(value != NULL);
    assert(cmp != NULL);

    char* it = first;
    while (it != last)
    {
        if (cmp(it, value) != 0)
            return it;
        it += stride;
    }
    return last;
}

static void reverse_(char* first, char* last, int stride)
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(stride > 0);

    while (first != last && first != last - stride)
    {
        swap(first, last - stride, stride);
        first += stride;
        last -= stride;
    }
}

void reverse(void* first, void* last, int stride)
{
    reverse_((char*)first, (char*)last, stride);
}

static opaque_vector duplicates_(char* first, char* last, int stride, int (*cmp)(void const*, void const*))
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(stride > 0);
    assert(cmp != NULL);

    opaque_vector ret = opaque_vector_create(sizeof(void*), opaque_vector_destroy_trivial);

    while (first != last)
    {
        char* next = find_not_cmp(first, last, stride, first, cmp);
        
        if (distance(first, next, stride) > 1)
            for (char* i = first; i != next; i += stride)
                opaque_vector_push(&ret, &(void*){i});

        first = next;
    }

    return ret;
}

opaque_vector duplicates(void* first, void* last, int stride, int (*cmp)(void const*, void const*))
{
    return duplicates_((char*)first, (char*)last, stride, cmp);
}

static opaque_vector excess_duplicates_(char* first, char* last, int stride, int (*cmp)(void const*, void const*))
{
    assert( (first == last) || (first != NULL && last != NULL) );
    assert(stride > 0);
    assert(cmp != NULL);

    opaque_vector ret = opaque_vector_create(sizeof(void*), opaque_vector_destroy_trivial);

    for (char* i = first; i != last;)
    {
        int lookahead = stride;
        while (i + lookahead != last)
        {
            if (cmp(i, i + lookahead) != 0)
                break;

            opaque_vector_push(&ret, &(void*){i + lookahead});
            lookahead += stride;
        }
        i += lookahead;
    }

    return ret;
}

opaque_vector excess_duplicates(void* first, void* last, int stride, int (*cmp)(void const*, void const*))
{
    return excess_duplicates_((char*)first, (char*)last, stride, cmp);
}

static opaque_vector unique_(char* first, char* last, int stride, int (*cmp)(void const*, void const*))
{
    assert(first != NULL);
    assert(last != NULL);
    assert(stride > 0);
    assert(cmp != NULL);

    opaque_vector ret = opaque_vector_create(sizeof(void*), opaque_vector_destroy_trivial);

    while (first != last)
    {
        opaque_vector_push(&ret, &(void*){first});
        first = find_not_cmp(first, last, stride, first, cmp);
    }

    return ret;
}

opaque_vector unique(void* first, void* last, int stride, int (*cmp)(void const*, void const*))
{
    return unique_((char*)first, (char*)last, stride, cmp);
}
