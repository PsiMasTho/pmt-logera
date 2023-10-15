//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser.ih"

#include "../log_data/log_data.h"
#include "../header_data/header_data.h"
#include "../date/date.h"

log_parser::log_parser(filesystem::path const& path, header_data const& headerData)
    : d_scanner(path)
    , d_matched { d_scanner.matched() }
    , d_logDataModifier(nullptr, headerData)
    , d_ret {nullptr}
    , d_errorInfo {nullopt}
{
}

unique_ptr<LogData> log_parser::gen()
{
    d_ret = make_unique<LogData>();
    d_logDataModifier.setTarget(d_ret.get());

    if (parse() == 0) // no error encountered
        return exchange(d_ret, nullptr);
    else
        return nullptr;
}

input_error const& log_parser::getErrorInfo() const
{
    return *d_errorInfo;
}

void log_parser::error()
{
    string matchTxt = d_matched;
    eraseAndReplace(&matchTxt, "\n", "*newline*");

    d_errorInfo.emplace(log_parse_error{
        d_scanner.filename(),
        "Unexpected input: (" + matchTxt + ") encountered.",
        d_scanner.lineNr()
    });
}

void log_parser::exceptionHandler(exception const& exc)
{
    d_errorInfo.emplace(log_parse_error{
        d_scanner.filename(),
        exc.what(),
        d_scanner.lineNr()
    });
    ABORT();
}