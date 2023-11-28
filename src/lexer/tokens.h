#pragma once

#include "../type_aliases.h"

#include <string>

struct log_tokens
{
    enum Tokens_ : token_t
    {
        IDENT = 257,
        NEWLINE,
        DATE,
        ATTR_VALUE,

        MAX_TOKEN
    };
};

struct header_tokens
{
    enum Tokens_ : token_t
    {
        DECL_VAR = 257,
        DECL_ATTR,
        IDENT,
        NEWLINE,
        REGEX,

        MAX_TOKEN
    };
};

struct token_record
{
    source_location_t loc;
    u16 length; // length of the string
    token_t tok;
};