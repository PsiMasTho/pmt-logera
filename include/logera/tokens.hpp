//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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
    char const* filename;
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
