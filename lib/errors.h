#pragma once

#include "tokens.h" // source_location

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define FATAL_ERROR(msg)\
{\
    fprintf(stderr, "fatal error: %s\n", msg); \
    abort();\
}

typedef struct file_error
{
    char* message; // owning
    source_location loc;
} file_error;

void
file_error_destroy(
    void* self
);

file_error
file_error_move(
    file_error* self
);

void
push_fmted_error(
    int errfmt_idx
,   char const** fmts
,   opaque_vector* errors
,   source_location location
,   ...
);
