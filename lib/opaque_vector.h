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

void
c_str_destroy( // for use with opaque_vector of char*, free() doesn't work
    void* self
);

void*         
opaque_vector_at(
    opaque_vector const* self
,   int pos
);

int           
opaque_vector_capacity(
    opaque_vector const* self
);

void          
opaque_vector_clear(
    opaque_vector* self
);

opaque_vector 
opaque_vector_create(
    int stride
,   void(*fn_destroy)(void*)
);

void          
opaque_vector_destroy_trivial(
    void* self
);

void          
opaque_vector_destroy(
    void* self
);

int // -> number of erased elements
opaque_vector_erase_if(
    opaque_vector* self
,   bool(*pred)(void const*)
);

void* // -> the next element
opaque_vector_erase_many(
    opaque_vector* self
,   void* first
,   void* last
); 

void* // -> the next element
opaque_vector_erase_one(
    opaque_vector* self
,   void* pos
);

void* // -> the first inserted element
opaque_vector_insert_many( // inserts before pos
    opaque_vector* self
,   void* pos
,   void const* first
,   void const* last
); 

void* // -> the inserted element
opaque_vector_insert_one( // inserts before pos
    opaque_vector* self
,   void* pos
,   void const* value
);

opaque_vector 
opaque_vector_move(
    opaque_vector* self
);

void          
opaque_vector_pop(
    opaque_vector* self
);

void          
opaque_vector_push(
    opaque_vector* self
,   void const* value
);

void          
opaque_vector_reserve(
    opaque_vector* self
,   int new_cap
);

int           
opaque_vector_size(
    opaque_vector const* self
);

