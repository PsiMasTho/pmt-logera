#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct opaque_vector
{
    void (*fn_destroy)(void*);
    void* begin;
    void* end;
    void* end_cap;
    int stride;
} opaque_vector;

/**
 * @brief Destructor function for opaque_vector of char*.
 */
void c_str_destroy(void* self);

void* opaque_vector_at(opaque_vector const* self, int pos);

int opaque_vector_capacity(opaque_vector const* self);

void opaque_vector_clear(opaque_vector* self);

opaque_vector opaque_vector_create(int stride, void (*fn_destroy)(void*));

void opaque_vector_destroy_trivial(void* self);

void opaque_vector_destroy(void* self);

/**
 * @brief Erases all elements that satisfy the predicate.
 * @return The number of erased elements.
 */
int opaque_vector_erase_if(opaque_vector* self, bool (*pred)(void const*));

/**
 * @brief Erases all elements in the range [first, last).
 * @return The next element after the last erased element.
 */
void* opaque_vector_erase_many(opaque_vector* self, void* first, void* last);

/**
 * @brief Erases the element at pos.
 * @return The next element after the erased element.
 */
void* // -> the next element
opaque_vector_erase_one(opaque_vector* self, void* pos);

/**
 * @brief Grows the vector at an exponential rate.
 */
void opaque_vector_grow(opaque_vector* self);

/**
 * @brief Inserts the elements in the range [first, last) before pos.
 * @return The first inserted element.
 */
void* opaque_vector_insert_many(opaque_vector* self, void* pos, void const* first, void const* last);

/**
 * @brief Inserts the element before pos.
 * @return The inserted element.
 */
void* opaque_vector_insert_one(opaque_vector* self, void* pos, void const* value);

opaque_vector opaque_vector_move(opaque_vector* self);

void opaque_vector_pop(opaque_vector* self);

void opaque_vector_push(opaque_vector* self, void const* value);

void opaque_vector_reserve(opaque_vector* self, int new_cap);

int opaque_vector_size(opaque_vector const* self);
