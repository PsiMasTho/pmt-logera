#pragma once

#include "flyweight_string.hpp"

namespace token
{

enum type
{
    COLON,
    SEMICOLON,
    NEWLINE,
    KW_ATTR,
    KW_VAR,
    IDENT,
    REGEX,
    ATTR_VALUE,
    DATE,

    _TYPE_COUNT
};

struct source_location
{
    char const* filename; // non-owning, todo: remove this redundancy
    int         line;
    int         column;
};

struct record
{
    source_location  location;
    flyweight_string lexeme;
    int              token;
};

} // namespace token
