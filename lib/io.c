#include "io.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

terminated_reader terminated_reader_create(
    FILE* const stream
,   char* const terminator_seq
,   int const terminator_seq_len
){
    assert(stream != NULL);
    assert((terminator_seq_len > 0 && terminator_seq != NULL) || terminator_seq_len == 0);

    return (terminated_reader) {
        .stream = stream,
        .terminator_seq = terminator_seq,
        .terminator_seq_len = terminator_seq_len,
        .wrote_eof = false
    };
}

int terminated_reader_read(
    terminated_reader* const reader
,   char* const buffer
,   int const bufsz
){
    assert(buffer != NULL);
    assert(bufsz > reader->terminator_seq_len);

    int read = fread(buffer, 1, bufsz - reader->terminator_seq_len, reader->stream);
    if (feof(reader->stream))
    {
        // write terminator if we have room
        int space = bufsz - read;
        if (space >= reader->terminator_seq_len)
        {
            memcpy(buffer + read, reader->terminator_seq, reader->terminator_seq_len);
            read += reader->terminator_seq_len;
            reader->wrote_eof = true;
        }
    }
    return read;
}

bool terminated_reader_done(
    terminated_reader const* const reader
){
    assert(reader != NULL);
    return reader->wrote_eof || terminated_reader_error(reader);
}

bool terminated_reader_error(
    terminated_reader const* const reader
){
    assert(reader != NULL);
    return ferror(reader->stream);
}

void basename_from_path(
    char const* const path
,   char* const dest
,   int const bufsz
){
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
