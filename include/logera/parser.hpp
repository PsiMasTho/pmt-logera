//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "ast.hpp"
#include "errors.hpp"
#include "lexer.hpp"
#include "logera/tokens.hpp"
#include "tokens.hpp"

class parser
{
    lexer&                          m_lexer;
    flyweight_string::storage_type& m_storage;
    error::container&               m_errors;

public:
    parser(lexer& lexer, flyweight_string::storage_type& storage, error::container& errors);

    auto parse() -> ast::file_node;

private:
    using parsed_line = std::variant<
        ast::date_node,
        ast::ident_value_pair_list_node,
        ast::decl_var_node,
        ast::decl_attr_node,
        ast::identifier_node,
        ast::entry_node,
        std::monostate>;

    auto parse_line(parsed_line* dest) -> parsed_line*;
    auto parse_declare_attribute(ast::decl_attr_node* dest) -> ast::decl_attr_node*;
    auto parse_declare_variable(ast::decl_var_node* dest) -> ast::decl_var_node*;
    auto parse_ident_value_pair_list(ast::ident_value_pair_list_node* dest) -> ast::ident_value_pair_list_node*;
    auto parse_ident_value_pair(ast::ident_value_pair_node* dest) -> ast::ident_value_pair_node*;
};

namespace error
{

struct unexpected_token : with_column, with_unformatted_msg
{
    unexpected_token(token::source_location location)
        : with_column{ location.filename, location.line, location.column }
        , with_unformatted_msg{ "unexpected token" }
    {
    }
};

} // namespace error