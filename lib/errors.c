#include "errors.h"

#include "opaque_vector.h"

#include <assert.h>
#include <string.h>

#define MAX_FMTED_ERRMSG_LEN 128

static file_error const file_error_default = {
    .message = NULL,
    .loc = {.line = 0, .column = 0}
};

void file_error_destroy(void* const self)
{
    assert(self != NULL);

    free(((file_error*)self)->message);
    *(file_error*)self = file_error_default;
}

file_error file_error_move(file_error* const self)
{
    assert(self != NULL);

    file_error ret = *self;
    *self = file_error_default;
    return ret;
}

void push_fmted_error(int const errfmt_idx, char const* const* const fmts, opaque_vector* const errors, source_location const location, ...)
{
    assert(errfmt_idx >= 0);
    assert(fmts != NULL);
    assert(errors != NULL);

    char msg[MAX_FMTED_ERRMSG_LEN];

    va_list args;
    va_start(args, location);
    vsnprintf(msg, MAX_FMTED_ERRMSG_LEN, fmts[errfmt_idx], args);
    va_end(args);

    opaque_vector_push(errors, &(file_error){ .message = strdup(msg), .loc = location });
}
