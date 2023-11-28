#pragma once

#include "../ast/log_nodes.h"
#include "typed_log_ast.h"

class lexed_buffer;

class typed_log_ast_builder
{
    lexed_buffer const& m_lex;
    typed_log_ast m_result;

    // std::vector<pair<string, u32> m_errors; // msg, line_nr

public:
    typed_log_ast_builder(lexed_buffer const& lex);

    void process(log_node const& node);
    auto release_result() -> typed_log_ast;

private:
    auto process(log_statement_node const& node) -> std::variant<std::vector<std::pair<std::string, std::string>>, std::string, log_date>;
    auto process(log_variable_node const& node) -> std::string;
    auto process(log_ident_value_pair_list_node const& node) -> std::vector<std::pair<std::string, std::string>>;
    auto process(log_date_node const& node) -> log_date;
    auto process(log_identifier_node const& node) -> std::string;
    auto process(log_ident_value_pair_node const& node) -> std::pair<std::string, std::string>;

    // helpers
    void set_date(log_date const& date);
    void add_new_entry(std::string const& var_name, std::vector<std::pair<std::string, std::string>>&& attr_vals);
};