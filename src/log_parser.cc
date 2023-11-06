//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser.ih"

log_parser::log_parser(std::filesystem::path const& path, log_parser_context& ctx)
    : d_scanner(path)
    , d_matched { d_scanner.matched() }
    , m_ctx {ctx}
    , m_error_info {nullopt}
{
}

unique_ptr<log_data> log_parser::gen()
{
    m_ctx.set_filename(d_scanner.filename());

    if (parse() == 0) // no error encountered
        return m_ctx.release_log_data();
    else
    {
        // if error encountered, then release the log_data
        // so that the context can be reused.
        m_ctx.release_log_data();
        return nullptr;
    }
}

parse_error const& log_parser::get_error_info() const
{
    return *m_error_info;
}

void log_parser::error()
{
    string matchTxt = d_matched;
    erase_and_replace(&matchTxt, "\n", "*newline*");

    m_error_info.emplace(parse_error{
        d_scanner.filename(),
        "Unexpected input: (" + matchTxt + ") encountered.",
        d_scanner.lineNr()
    });
}

void log_parser::exceptionHandler(exception const& exc)
{
    m_error_info.emplace(parse_error{
        d_scanner.filename(),
        exc.what(),
        d_scanner.lineNr()
    });
    ABORT();
}