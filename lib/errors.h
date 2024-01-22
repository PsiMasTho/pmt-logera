#pragma once

#include "tokens.h" // source_location

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define FATAL_ERROR(msg)                           \
    {                                              \
        fprintf(stderr, "fatal error: %s\n", msg); \
        abort();                                   \
    }

typedef struct opaque_vector opaque_vector;

typedef struct file_error
{
    char* message; // owning
    source_location loc;
} file_error;

void file_error_destroy(void* self);

file_error file_error_move(file_error* self);

/**
 * @brief Pushes a formatted error message into the error vector.
 *
 * @param errfmt_idx The index of the error format string in the `fmts` array.
 * @param fmts An array of error format strings.
 * @param errors The error vector to push the formatted error message into.
 * @param location The source location where the error occurred. If the filename is NULL, the
 *                location is ignored.
 * @param ... Variadic arguments to be used in formatting the error message.
 */
void push_fmted_error(int errfmt_idx, char const* const* fmts, opaque_vector* errors, source_location location, ...);
