#include "opaque_vector.h"

#include "algo.h"
#include "errors.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static int next_capacity(int capacity);

// ----------------------------------------------------------------------------

void c_str_destroy(void* self)
{
    assert(self != NULL);

    char** const str = (char**)self;
    free(*str);
    *str = NULL;
}

void* opaque_vector_at(opaque_vector const* self, int pos)
{
    assert(self != NULL);
    assert(pos >= 0);
    assert(pos <= opaque_vector_size(self));

    if (pos == 0)
        return self->begin;
    else
    {
        assert(self->begin != NULL);
        return advance(self->begin, pos, self->stride);
    }
}

int opaque_vector_capacity(opaque_vector const* self)
{
    assert(self != NULL);

    if (self->begin == NULL)
        return 0;
    else
        return distance(self->begin, self->end_cap, self->stride);
}

void opaque_vector_clear(opaque_vector* self)
{
    assert(self != NULL);

    while (self->end != self->begin)
        opaque_vector_pop(self);
}

opaque_vector opaque_vector_create(int stride, void (*fn_destroy)(void*))
{
    assert(stride > 0);
    assert(fn_destroy != NULL);

    opaque_vector self = { .fn_destroy = fn_destroy, .begin = NULL, .end = NULL, .end_cap = NULL, .stride = stride };
    return self;
}

void opaque_vector_destroy_trivial(void* self)
{
    (void)self;
}

void opaque_vector_destroy(void* self)
{
    assert(self != NULL);

    opaque_vector* const vec = (opaque_vector*)self;

    opaque_vector_clear(vec);

    free(vec->begin);
    *vec = opaque_vector_create(vec->stride, vec->fn_destroy);
}

int opaque_vector_erase_if(opaque_vector* self, bool (*pred)(void const*))
{
    assert(self != NULL);
    assert(pred != NULL);
    char* first = self->begin;
    char* last = self->end;

    first = find_if(first, last, self->stride, pred);
    if (first == last)
        return 0;

    self->fn_destroy(first);

    int cnt = 1;
    for (char* i = advance(first, 1, self->stride); i != last; i = advance(i, 1, self->stride))
    {
        if (!pred(i))
        {
            memcpy(first, i, self->stride);
            first = advance(first, 1, self->stride);
        }
        else
        {
            self->fn_destroy(i);
            ++cnt;
        }
    }

    self->end = first;
    return cnt;
}

void* opaque_vector_erase_many(opaque_vector* self, void* first, void* last)
{
    assert(self != NULL);
    assert(first != NULL);
    assert(last != NULL);
    assert(self->fn_destroy != NULL);
    assert(self->begin <= first);
    assert(first < last);
    assert(last <= self->end);
    assert(((char*)first - (char*)self->begin) % self->stride == 0);
    assert(((char*)last - (char*)self->begin) % self->stride == 0);

    for (void* pos = first; pos != last; pos = advance(pos, 1, self->stride))
        self->fn_destroy(pos);

    memmove(first, last, distance(last, self->end, self->stride));

    self->end = advance(self->end, -distance(first, last, self->stride), self->stride);

    return first;
}

void* opaque_vector_erase_one(opaque_vector* self, void* pos)
{
    assert(self != NULL);
    assert(pos != NULL);
    assert(self->fn_destroy != NULL);
    assert(self->begin <= pos);
    assert(pos < self->end);
    assert(((char*)pos - (char*)self->begin) % self->stride == 0);

    self->fn_destroy(pos);
    self->end = advance(self->end, 1, -self->stride);
    memmove(pos, advance(pos, 1, self->stride), distance(pos, self->end, 1));

    return pos;
}

void opaque_vector_grow(opaque_vector* self)
{
    opaque_vector_reserve(self, next_capacity(opaque_vector_capacity(self)));
}

void* opaque_vector_insert_many(opaque_vector* self, void* pos, void const* first, void const* last)
{
    assert(self != NULL);
    assert(self->begin <= pos && pos <= self->end);
    assert(first != NULL);
    assert(last != NULL);
    assert(first <= last);

    int const n_items = distance(first, last, self->stride);
    int const offset = distance(self->begin, pos, self->stride);

    // reserve space
    if (opaque_vector_size(self) + n_items > opaque_vector_capacity(self))
        opaque_vector_reserve(self, next_capacity(opaque_vector_capacity(self) + n_items));

    pos = advance(self->begin, offset, self->stride);

    // move elements
    memmove(advance(pos, n_items, self->stride), pos, (char*)self->end - (char*)pos);

    // copy elements
    memcpy(pos, first, n_items * self->stride);

    self->end = advance(self->end, n_items, self->stride);

    return pos;
}

void* opaque_vector_insert_one(opaque_vector* self, void* pos, void const* value)
{
    assert(self != NULL);
    assert(self->begin <= pos && pos <= self->end);
    assert(value != NULL);

    int const offset = distance(self->begin, pos, self->stride);

    // reserve space
    if (self->end == self->end_cap)
        opaque_vector_reserve(self, next_capacity(opaque_vector_capacity(self)));

    // may have been reallocated
    pos = advance(self->begin, offset, self->stride);

    // move elements
    memmove(advance(pos, 1, self->stride), pos, (char*)self->end - (char*)pos);

    // copy element
    memcpy(pos, value, self->stride);

    self->end = advance(self->end, 1, self->stride);

    return pos;
}

opaque_vector opaque_vector_move(opaque_vector* self)
{
    assert(self != NULL);

    opaque_vector new_self = *self;
    *self = opaque_vector_create(self->stride, self->fn_destroy);
    return new_self;
}

void opaque_vector_pop(opaque_vector* self)
{
    assert(self != NULL);
    assert(self->fn_destroy != NULL);
    assert(opaque_vector_size(self) > 0);

    self->end = advance(self->end, 1, -self->stride);
    self->fn_destroy(self->end);
}

void opaque_vector_push(opaque_vector* self, void const* value)
{
    assert(self != NULL);
    assert(value != NULL);

    if (self->end == self->end_cap)
        opaque_vector_grow(self);

    memcpy(self->end, value, self->stride);
    self->end = advance(self->end, 1, self->stride);
}

void opaque_vector_reserve(opaque_vector* self, int new_cap)
{
    assert(self != NULL);

    if (new_cap <= opaque_vector_capacity(self))
        return;

    int const old_size = opaque_vector_size(self);

    void* const new_begin = realloc(self->begin, new_cap * self->stride);

    if (new_begin == NULL)
        FATAL_ERROR("realloc() failed");

    self->begin = new_begin;
    self->end = advance(self->begin, old_size, self->stride);
    self->end_cap = advance(self->begin, new_cap, self->stride);
}

int opaque_vector_size(opaque_vector const* self)
{
    assert(self != NULL);

    if (self->begin == NULL)
        return 0;
    else
        return distance(self->begin, self->end, self->stride);
}

// ----------------------------------------------------------------------------

static int next_capacity(int capacity)
{
    return capacity == 0 ? 1 : capacity * 2;
}
