#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef struct terminated_reader
{
    FILE* stream;         // not owning
    char* terminator_seq; // not owning, not null terminated
    int terminator_seq_len;
    bool wrote_eof;
} terminated_reader;

terminated_reader
terminated_reader_create(
    FILE* stream
,   char* terminator_seq   // not null-terminated
,   int terminator_seq_len // may be 0
);

int // -> number of bytes read
terminated_reader_read( 
    terminated_reader* reader
,   char* buffer
,   int bufsz
);

bool // -> if EOF or error
terminated_reader_done(
    terminated_reader const* reader
);

bool // -> if error
terminated_reader_error(
    terminated_reader const* reader
);

void
basename_from_path(
    char const* path
,   char* dest // output buffer
,   int bufsz  // size of output buffer, must be >= 1
);
