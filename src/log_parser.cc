//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser.ih"

#include "log_data.h"
#include "header_data.h"
#include "date.h"

log_parser::log_parser(filesystem::path const& path, header_data const& hd)
    : d_scanner(path)
    , d_matched { d_scanner.matched() }
    , d_logDataModifier(nullptr, hd)
    , d_ret {nullptr}
    , d_errorInfo {nullopt}
{
}

unique_ptr<log_data> log_parser::gen()
{
    d_ret = make_unique<log_data>();
    d_logDataModifier.set_target(d_ret.get());
    d_logDataModifier.set_filename(d_scanner.filename());

    if (parse() == 0) // no error encountered
        return exchange(d_ret, nullptr);
    else
        return nullptr;
}

parse_error const& log_parser::get_error_info() const
{
    return *d_errorInfo;
}

void log_parser::error()
{
    string matchTxt = d_matched;
    erase_and_replace(&matchTxt, "\n", "*newline*");

    d_errorInfo.emplace(parse_error{
        d_scanner.filename(),
        "Unexpected input: (" + matchTxt + ") encountered.",
        d_scanner.lineNr()
    });
}

void log_parser::exceptionHandler(exception const& exc)
{
    d_errorInfo.emplace(parse_error{
        d_scanner.filename(),
        exc.what(),
        d_scanner.lineNr()
    });
    ABORT();
}