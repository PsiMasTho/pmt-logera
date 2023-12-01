#pragma once

#include "../common_types.h"
#include <vector>
#include <optional>
#include <variant>

template <typename T>
struct container_node
{
    std::vector<T> children;
};

struct leaf_node
{
    tok_rec_idx_t token_rec_idx; // index of the token_record in the lexed_buffer
};

using ast_node = std::variant<struct root_node,
                              struct statement_node,
                              struct ident_value_pair_list_node,
                              struct ident_value_pair_node,
                              struct date_node,
                              struct attr_value_node,
                              struct identifier_node,
                              struct decl_var_node,
                              struct decl_attr_node,
                              struct regex_node>;


struct root_node : container_node<ast_node>
{ };

struct statement_node : container_node<ast_node>
{ };

struct ident_value_pair_list_node : container_node<ast_node>
{ };

struct ident_value_pair_node : container_node<ast_node>
{ };

struct date_node : leaf_node
{ };

struct attr_value_node : leaf_node
{ };

struct identifier_node : leaf_node
{ };

struct decl_var_node : container_node<ast_node>
{ };

struct decl_attr_node : container_node<ast_node>
{ };

struct regex_node : leaf_node
{ };