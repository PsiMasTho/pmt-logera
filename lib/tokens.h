#pragma once

enum token_type
{
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_NEWLINE,
    TOKEN_KW_ATTR,
    TOKEN_KW_VAR,
    TOKEN_IDENT,
    TOKEN_REGEX,
    TOKEN_ATTR_VALUE,
    TOKEN_DATE,

    _TOKEN_COUNT
};

extern char const* const token_names[_TOKEN_COUNT];

typedef struct source_location
{
    char const* filename; // non-owning
    int line;
    int column;
} source_location;

typedef struct token_record
{
    source_location location;
    char* lexeme; // owning, NTBS or NULL
    int token;
} token_record;

token_record token_record_copy(token_record const* self);

token_record token_record_move(token_record* self);

void token_record_destroy(void* self);
