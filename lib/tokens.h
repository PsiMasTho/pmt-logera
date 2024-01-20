#pragma once

#include "opaque_vector.h"

enum token_type
{
    TOKEN_NEWLINE,
    TOKEN_COLON,
    TOKEN_DECL_ATTR,
    TOKEN_SEMICOLON,
    TOKEN_DECL_VAR,
    TOKEN_REGEX,
    TOKEN_ATTR_VALUE,
    TOKEN_DATE,
    TOKEN_IDENT,

    _TOKEN_COUNT
};

extern char const *const token_names[_TOKEN_COUNT];

typedef struct
{
    char const* filename; // non-owning
    int line;
    int column;
} source_location;

typedef struct
{
    source_location loc;
    char*           lexeme; // owning, NTBS or NULL
    int             token;
} token_record;

source_location
source_location_create(
    char const* filename
,   int line
,   int column
);

char const*
token_name(
    int tok
);

token_record
token_record_copy(
    token_record const* self
);

void
token_record_destroy(
    void* self
);

token_record
token_record_move(
    token_record* self
);

