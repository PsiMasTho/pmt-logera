#pragma once

#include "node_common.h"
#include <variant>

using log_node = std::variant<struct log_root_node,
                              struct log_statement_node,
                              struct log_variable_node,
                              struct log_ident_value_pair_list_node,
                              struct log_date_node,
                              struct log_identifier_node,
                              struct log_ident_value_pair_node>;

struct log_root_node : container_node<log_node>
{ };

struct log_statement_node : container_node<log_node>
{ };

struct log_variable_node : container_node<log_node>
{ };

struct log_ident_value_pair_list_node : container_node<log_node>
{ };

struct log_date_node : leaf_node
{ };

struct log_identifier_node : leaf_node
{ };

struct log_ident_value_pair_node : leaf_node
{ };