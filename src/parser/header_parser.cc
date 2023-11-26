//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "header_parser.h"

#include "../utility/utility.h"

#include <fmt/format.h>

using namespace std;

header_parser::header_parser(lexed_file const& file)
    : m_walker(file)
    , m_ast(ast<ast_node>{})
{
    m_ast.add_node(ast_node{0, static_cast<u8>(header_node_enum::ROOT)});
}

auto header_parser::gen() -> optional<ast<ast_node>>
{
    if(parse() == 0) // no error encountered
        return std::move(m_ast);
    else
        return nullopt;
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