//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_parser.h"

#include "../utility/utility.h"

#include <fmt/format.h>

using namespace std;

log_parser::log_parser(lexed_file_walker& walker, log_parser_context& ctx)
    : m_walker(walker)
    , m_ctx{ctx}
{ }

unique_ptr<log_data> log_parser::gen()
{
    m_ctx.set_lexed_file_walker(m_walker.get_file());
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
    string match_txt{m_walker.get_cur_match().data(), m_walker.get_cur_match().size()};

    if(match_txt.empty())
        match_txt = "<EOF>";
    else
        erase_and_replace(&match_txt, "\n", "*newline*");

    m_ctx.push_error(
        parse_error::SYNTAX, m_walker.get_file().get_filename(), fmt::format("Unexpected input: {} encountered.", match_txt), m_walker.get_cur_line_nr());
    log_parser_base::ABORT();
}

void log_parser::exceptionHandler(exception const& exc)
{
    m_ctx.push_error(parse_error::EXCEPTION, m_walker.get_file().get_filename(), exc.what(), m_walker.get_cur_line_nr());
    log_parser_base::ABORT();
}