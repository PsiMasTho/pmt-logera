//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_ARG_PARSER_H
#define INCLUDED_ARG_PARSER_H

#include "../args/args.h"

struct DefaultArgParser : ArgParserCore
{
    DefaultArgParser(char const* const* argv)
        : ArgParserCore{argv}
    { }

    std::pair<std::variant<char, std::string>, std::string> next();
    bool done() const;
};

#endif