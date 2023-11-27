#pragma once

#include "node_common.h"
#include <variant>

using header_node = std::variant<struct header_root_node,
                                 struct header_statement_node,
                                 struct header_decl_var_node,
                                 struct header_decl_attr_node,
                                 struct header_identifier_node,
                                 struct header_regex_node>;

struct header_root_node : container_node<header_node>
{ };

struct header_statement_node : container_node<header_node>
{ };

struct header_decl_var_node : container_node<header_node>
{ };

struct header_decl_attr_node : container_node<header_node>
{ };

struct header_identifier_node : leaf_node
{ };

struct header_regex_node : leaf_node
{ };