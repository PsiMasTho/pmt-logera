#pragma once

#include "opaque_vector.h"

#include <stddef.h>
#include <stdbool.h>

void
swap( // lhs == rhs will do nothing
    void* lhs
,   void* rhs
,   int stride
);

int // -> how many strides from first to last, negative is allowed
distance(
    void const* first
,   void const* last
,   int stride
);

bool // -> if target is in [first, last)
between(
    void const* first // must be <= last
,   void const* last
,   void const* target
);

void*
advance(
    void const* it
,   int n // negative is allowed
,   int stride
);

void*
find_val( // -> first element in [first, last) that is equal to value, or last if not found
    void const* first
,   void const* last
,   void const* value
,   int stride);

void*
find_not_val(
    void const* first
,   void const* last
,   void const* value
,   int stride);

void*
find_if(
    void const* first
,   void const* last
,   int stride
,   bool (*pred)(void const*)
);

void*
find_if_not(
    void const* first
,   void const* last
,   int stride,
    bool (*pred)(void const*)
);

void*
find_cmp(
    void const* first
,   void const* last
,   int stride
,   void const* value
,   int (*cmp)(void const*, void const*)
);

void*
find_not_cmp(
    void const* first
,   void const* last
,   int stride
,   void const* value
,   int (*cmp)(void const*, void const*)
);

void
reverse(
    void* first
,   void* last
,   int stride
);

opaque_vector // -> vector of void* of elements that appear more than once in a sorted range in ascending order, including the first ones
duplicates(
    void* first
,   void* last
,   int stride
,   int (*cmp)(void const*, void const*)
);

opaque_vector // -> vector of void* of elements that appear more than once in a sorted range in ascending order, excluding the first ones
excess_duplicates(
    void* first
,   void* last
,   int stride
,   int (*cmp)(void const*, void const*)
);

opaque_vector // -> vector of void* to first unique elements in a sorted range in ascending order
unique(
    void* first
,   void* last
,   int stride
,   int (*cmp)(void const*, void const*)
);
