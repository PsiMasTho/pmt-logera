#pragma once

#include "ast.hpp"
#include "errors.hpp"
#include "lexer.hpp"

class parser
{
    lexer&                          m_lexer;
    flyweight_string::storage_type& m_storage;
    std::vector<error::record>&     m_errors;

public:
    parser(lexer& lexer, flyweight_string::storage_type& storage, std::vector<error::record>& errors);

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
