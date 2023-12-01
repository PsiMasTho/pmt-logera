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
    // wrapping annot_tok so that std::variant can distinguish between them
    struct annot_str : annot_tok
    {};

    struct annot_date : annot_tok
    {};

    void process(log_root_node const& node);
    auto process(log_statement_node const& node) -> std::variant<std::vector<std::pair<annot_tok, annot_tok>>, annot_str, annot_date>;
    auto process(log_variable_node const& node) -> annot_str;
    auto process(log_ident_value_pair_list_node const& node) -> std::vector<std::pair<annot_tok, annot_tok>>;
    auto process(log_ident_value_pair_node const& node) -> std::pair<annot_tok, annot_tok>;
    auto process(log_date_node const& node) -> annot_date;
    auto process(log_attr_value_node const& node) -> annot_str;
    auto process(log_identifier_node const& node) -> annot_str;

    // helpers
    void set_date(annot_date date);
    void add_new_entry(annot_str var, std::vector<std::pair<annot_tok, annot_tok>>&& pairs);
};