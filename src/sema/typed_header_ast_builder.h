#pragma once

#pragma once

#include "../ast/header_nodes.h"
#include "typed_header_ast.h"

class lexed_buffer;

class typed_header_ast_builder
{
    lexed_buffer const& m_lex;
    typed_header_ast m_result;

    // std::vector<pair<string, u32> m_errors; // msg, line_nr

public:
    typed_header_ast_builder(lexed_buffer const& lex);

    void process(header_node const& node);
    auto release_result() -> typed_header_ast;

private:
    void process(header_root_node const& node);
    auto process(header_statement_node const& node) -> std::variant<decl_attr_statement, decl_var_statement>;
    auto process(header_decl_var_node const& node) -> decl_var_statement;
    auto process(header_decl_attr_node const& node) -> decl_attr_statement;
    auto process(header_identifier_node const& node) -> u32;
    auto process(header_regex_node const& node) -> u32;

    // helpers
    void add_decl_attr_statement(decl_attr_statement&& statement);
    void add_decl_var_statement(decl_var_statement&& statement);
};