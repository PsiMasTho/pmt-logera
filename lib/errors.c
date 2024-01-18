#include "errors.h"

#include <assert.h>

static file_error const file_error_default = { .message = NULL, .loc = { .line = 0, .column = 0 } };

void file_error_destroy(
    void* const self
){
    assert(self != NULL);

    free(((file_error*)self)->message);
    *(file_error*)self = file_error_default;
}

file_error file_error_move(
    file_error* const self
){
    assert(self != NULL);

    file_error ret = *self;
    *self = file_error_default;
    return ret;
}
