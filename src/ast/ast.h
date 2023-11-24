#pragma once

#include "../lexer/lexed_file.h"

#include <variant>
#include <vector>

using offset_t = uint32_t;

using header_file_node = std::variant<
    struct variable_node,
    struct regex_node,
    struct decl_var_node,
    struct eof_node>;

using log_file_node = std::variant<
    struct date_node,
    struct variable_node,
    struct newline_node,
    struct eof_node,
    struct ident_value_pair_node,
    struct ident_value_pair_list_node>;

struct date_node
{
    offset_t m_date;
};

struct variable_node
{
    offset_t m_identifier;
};

struct newline_node
{
    offset_t m_newline;
};

struct eof_node
{
    offset_t m_eof;
};

struct ident_value_pair_node
{
    offset_t m_pair;
    // variable_node m_variable;
    // value_node m_value;
};

struct ident_value_pair_list_node
{
    std::vector<ident_value_pair_node> m_pairs;
};

struct header_file_ast
{
    offset_t m_lexed_file;
    std::vector<header_file_node> m_nodes;
};

struct log_file_ast
{
    offset_t m_lexed_file;
    std::vector<log_file_node> m_nodes;
};