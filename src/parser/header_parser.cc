//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "header_parser.h"

#include "../utility/utility.h"

#include <fmt/format.h>

using namespace std;

header_parser::header_parser(lexed_buffer const& file)
    : m_walker(file)
    , m_ast(header_root_node{})
{}

auto header_parser::release_result() -> header_node
{
    parse();
    return std::move(m_ast);
}

void header_parser::error()
{
    // string match_txt{m_walker.get_cur_match().data(), m_walker.get_cur_match().size()};

    // if(match_txt.empty())
    //     match_txt = "<EOF>";
    // else
    //     erase_and_replace(&match_txt, "\n", "*newline*");

    // m_ctx.push_error(
    //     parse_error::SYNTAX, m_walker.get_file().get_filename(), fmt::format("Unexpected input: {} encountered.", match_txt), m_walker.get_cur_line_nr());
    header_parser_base::ABORT();
}

void header_parser::exceptionHandler(exception const& exc)
{
    // m_ctx.push_error(parse_error::EXCEPTION, m_walker.get_file().get_filename(), exc.what(), m_walker.get_cur_line_nr());
    header_parser_base::ABORT();
}