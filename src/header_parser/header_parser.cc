//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "header_parser.ih"

#include "../header_data/header_data.h"
#include <utility>

HeaderParser::HeaderParser(filesystem::path const& path)
    : d_scanner(path)
    , d_matched(d_scanner.matched())
    , d_ret { nullptr }
{
}

unique_ptr<HeaderData> HeaderParser::gen()
{
    d_ret = make_unique<HeaderData>();

    if (parse() == 0) // no error encountered
        return exchange(d_ret, nullptr);
    else
        return nullptr;
}

void HeaderParser::error()
{
    cerr << "Syntax error in header file: " << d_scanner.filename() << '\n';

    string matchTxt = d_matched;

    eraseAndReplace(&matchTxt, "\n", "*newline*");

    cerr << "Unexpected input: (" << matchTxt << ") encountered.\n";
    cerr << "At line: " << d_scanner.lineNr() << '\n';
}

void HeaderParser::exceptionHandler(exception const& exc)
{
    cerr << "Error in header file: " << d_scanner.filename() << '\n';
    cerr << '\t' << exc.what() << '\n';
    cerr << "At line: " << d_scanner.lineNr() << '\n';
    ABORT();
}