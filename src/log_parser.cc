//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser.ih"

log_parser::log_parser(std::filesystem::path const& path, log_parser_context& ctx)
    : d_scanner(path)
    , d_matched{d_scanner.matched()}
    , m_ctx{ctx}
{ }

unique_ptr<log_data> log_parser::gen()
{
    m_ctx.set_scanner(d_scanner);
    if(parse() == 0) // no error encountered
        return m_ctx.release_log_data();
    else
    {
        // if error encountered, then release the log_data
        // so that the context can be reused.
        m_ctx.release_log_data();
        return nullptr;
    }
}

void log_parser::error()
{
    string match_txt = d_matched;

    if(match_txt.empty())
        match_txt = "<EOF>";
    else
        erase_and_replace(&match_txt, "\n", "*newline*");

    m_ctx.push_error(
        parse_error::SYNTAX, d_scanner.filename(), fmt::format("Unexpected input: {} encountered.", match_txt), d_scanner.lineNr());
    log_parser_base::ABORT();
}

void log_parser::exceptionHandler(exception const& exc)
{
    m_ctx.push_error(parse_error::EXCEPTION, d_scanner.filename(), exc.what(), d_scanner.lineNr());
    log_parser_base::ABORT();
}