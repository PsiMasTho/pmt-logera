//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "header_parser.h"

#include "utility.h"

#include <fmt/format.h>

using namespace std;

header_parser::header_parser(std::filesystem::path const& path, header_parser_context& ctx)
    : m_lexer(path.c_str())
    , m_ctx(ctx)
{ }

unique_ptr<header_data> header_parser::gen()
{
    m_ctx.set_lexer(m_lexer);
    if(parse() == 0) // no error encountered
        return m_ctx.release_header_data();
    else
        return nullptr;
}

void header_parser::error()
{
    string match_txt{m_lexer.matched().data(), m_lexer.matched().size()};

    if(match_txt.empty())
        match_txt = "<EOF>";
    else
        erase_and_replace(&match_txt, "\n", "*newline*");

    m_ctx.push_error(
        parse_error::SYNTAX, m_lexer.filename(), fmt::format("Unexpected input: {} encountered.", match_txt), m_lexer.line_nr());
    header_parser_base::ABORT();
}

void header_parser::exceptionHandler(exception const& exc)
{
    m_ctx.push_error(parse_error::EXCEPTION, m_lexer.filename(), exc.what(), m_lexer.line_nr());
    header_parser_base::ABORT();
}