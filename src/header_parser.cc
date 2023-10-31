//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "header_parser.ih"

#include "header_data.h"
#include <utility>

header_parser::header_parser(filesystem::path const& path)
    : d_scanner(path)
    , d_matched(d_scanner.matched())
    , d_ret {nullptr}
    , d_errorInfo {nullopt}
{
}

unique_ptr<header_data> header_parser::gen()
{
    d_ret = make_unique<header_data>();

    if (parse() == 0) // no error encountered
        return exchange(d_ret, nullptr);
    else
        return nullptr;
}

parse_error const& header_parser::get_error_info() const
{
    return *d_errorInfo;
}

void header_parser::error()
{
    string matchTxt = d_matched;
    erase_and_replace(&matchTxt, "\n", "*newline*");

    d_errorInfo.emplace(parse_error{
        d_scanner.filename(),
        "Unexpected input: (" + matchTxt + ") encountered.",
        d_scanner.lineNr()
    });
}

void header_parser::exceptionHandler(exception const& exc)
{
    d_errorInfo.emplace(parse_error{
        d_scanner.filename(),
        exc.what(),
        d_scanner.lineNr()
    });
    ABORT();
}