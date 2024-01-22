#include "io.h"

#include "algo.h"
#include "errors.h"
#include "opaque_vector.h"
#include "tokens.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define READALLF_MIN_INITIAL_BUFFER_SIZE 65536

enum readallf_error
{
    READALLF_ERROR_STREAM_ERROR,
    READALLF_ERROR_CANNOT_OPEN_FILE,

    _READALLF_ERROR_COUNT
};

static char const* const readallf_error_messages[_READALLF_ERROR_COUNT] = {
    [READALLF_ERROR_STREAM_ERROR] = "stream error for file \'%s\'",
    [READALLF_ERROR_CANNOT_OPEN_FILE] = "cannot open file \'%s\'",
};

void basename_from_path(char const* const path, char* const dest, int const bufsz)
{
    assert(path != NULL);
    assert(dest != NULL);
    assert(bufsz >= 1);

    char const* last_slash = strrchr(path, '/');

    if (last_slash == NULL)
        last_slash = strrchr(path, '\\');

    if (last_slash == NULL)
        strncpy(dest, path, bufsz);
    else
        strncpy(dest, last_slash + 1, bufsz);
}

bool readallf(char const* const path, opaque_vector* const buffer, opaque_vector* const errors)
{
    assert(buffer != NULL);
    assert(errors != NULL);

    FILE* const fp = fopen(path, "rb");

    if (fp == NULL)
    {
        push_fmted_error(
            READALLF_ERROR_CANNOT_OPEN_FILE,
            readallf_error_messages,
            errors,
            (source_location){ .filename = NULL, .line = 0, .column = 0 },
            path);
        return false;
    }

    if (ferror(fp))
    {
        push_fmted_error(
            READALLF_ERROR_STREAM_ERROR,
            readallf_error_messages,
            errors,
            (source_location){ .filename = NULL, .line = 0, .column = 0 },
            path);
        fclose(fp);
        return false;
    }

    opaque_vector_clear(buffer);

    if (opaque_vector_capacity(buffer) < READALLF_MIN_INITIAL_BUFFER_SIZE)
        opaque_vector_reserve(buffer, READALLF_MIN_INITIAL_BUFFER_SIZE);

    while (1)
    {
        int const n = fread(buffer->end, 1, opaque_vector_capacity(buffer) - opaque_vector_size(buffer), fp);

        if (!n)
            break;

        buffer->end = advance(buffer->end, n, 1);

        if (buffer->end == buffer->end_cap)
            opaque_vector_grow(buffer);
    }

    if (ferror(fp))
    {
        push_fmted_error(
            READALLF_ERROR_STREAM_ERROR,
            readallf_error_messages,
            errors,
            (source_location){ .filename = NULL, .line = 0, .column = 0 },
            path);
        fclose(fp);
        return false;
    }

    fclose(fp);

    opaque_vector_push(buffer, &(char){ '\0' });

    return true;
}
