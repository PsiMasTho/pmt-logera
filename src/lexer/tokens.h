#pragma once

#include "../common_types.h"

#include <string>

struct tokens
{
    enum Tokens_ : tok_t
    {
        DECL_VAR = 257,
        DECL_ATTR,
        DATE,
        ATTR_VALUE,
        IDENT,
        REGEX,

        MAX_TOKEN
    };
};

struct token_record
{
    source_location_t loc;
    u16 length; // length of the string
    tok_t tok;
};