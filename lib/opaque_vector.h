#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct opaque_vector
{
    void (*fn_destroy)(void*);
    void*  begin;
    void*  end;
    void*  end_cap;
    int    stride;
} opaque_vector;

opaque_vector opaque_vector_create(int stride, void(*fn_destroy)(void*));
opaque_vector opaque_vector_move(opaque_vector* self);
void          opaque_vector_destroy(void* self);
void*         opaque_vector_insert_one(opaque_vector* self, void* pos, void const* value); // inserts before pos, returns the element
void*         opaque_vector_insert_many(opaque_vector* self, void* pos, void const* first, void const* last); // inserts before pos, returns the first element
void          opaque_vector_push(opaque_vector* self, void const* value);
void          opaque_vector_pop(opaque_vector* self);
void          opaque_vector_clear(opaque_vector* self);
void*         opaque_vector_erase_one(opaque_vector* self, void* pos); // returns the next element
void*         opaque_vector_erase_many(opaque_vector* self, void* first, void* last); // returns the next element
int           opaque_vector_erase_if(opaque_vector* self, bool(*pred)(void const*)); // returns the number of erased elements
void          opaque_vector_reserve(opaque_vector* self, int new_cap);
void*         opaque_vector_at(opaque_vector const* self, int pos);
int           opaque_vector_size(opaque_vector const* self);
int           opaque_vector_capacity(opaque_vector const* self);

void opaque_vector_destroy_trivial(void* self);

void c_str_destroy(void* self); // when self is char**. fn_destroy free() instead of this would cause a double free
