//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

struct parse_error
{
    enum : int
    {
        SEMANTIC,
        SYNTAX,
        EXCEPTION
    };

    int error_type;
    std::string filename;
    std::string msg;
    std::size_t line_nr;
};