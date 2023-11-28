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
    // wrapping primitives so that std::variant can distinguish between them
    struct string_u32
    {
        u32 val;
        operator u32() const
        {
            return val;
        }
    };

    struct date_u32
    {
        u32 val;
        operator u32() const
        {
            return val;
        }
    };

    void process(log_root_node const& node);
    auto process(log_statement_node const& node) -> std::variant<std::vector<std::pair<u32, u32>>, string_u32, date_u32>;
    auto process(log_variable_node const& node) -> string_u32;
    auto process(log_ident_value_pair_list_node const& node) -> std::vector<std::pair<u32, u32>>;
    auto process(log_ident_value_pair_node const& node) -> std::pair<u32, u32>;
    auto process(log_date_node const& node) -> date_u32;
    auto process(log_attr_value_node const& node) -> string_u32;
    auto process(log_identifier_node const& node) -> string_u32;

    // helpers
    void set_date(date_u32 date);
    void add_new_entry(string_u32 var, std::vector<std::pair<u32, u32>>&& pairs);
};