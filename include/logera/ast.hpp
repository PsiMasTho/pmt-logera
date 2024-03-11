//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "meta.hpp"
#include "tokens.hpp"

#include <string>
#include <variant>
#include <vector>

namespace ast
{

struct regex_node
{
    token::record record;
};

struct identifier_node
{
    token::record record;
};

struct attr_value_node
{
    token::record record;
};

struct date_node
{
    token::record record;
};

struct decl_attr_node
{
    identifier_node         identifier;
    std::vector<regex_node> children;
};

struct decl_var_node
{
    identifier_node              identifier;
    std::vector<identifier_node> children;
};

struct ident_value_pair_node
{
    identifier_node identifier;
    attr_value_node attr_value;
};

struct ident_value_pair_list_node
{
    std::vector<ident_value_pair_node> children;
};

struct entry_node
{
    identifier_node            identifier;
    ident_value_pair_list_node ident_value_pair_list;
};

struct file_node
{
    using children_type = std::
        variant<date_node, ident_value_pair_list_node, decl_var_node, decl_attr_node, identifier_node, entry_node>;

    template <typename T>
    static constexpr auto child_index_v = meta::get_index<T, children_type>::value;

    std::string                filename;
    std::vector<children_type> children;
};

struct multifile_node
{
    std::vector<file_node> children;
};

template <typename T>
concept decl_node = std::is_same_v<T, decl_attr_node> || std::is_same_v<T, decl_var_node>;

template <typename T>
concept node = std::is_same_v<T, regex_node> || std::is_same_v<T, identifier_node> || std::is_same_v<T, attr_value_node>
               || std::is_same_v<T, date_node> || std::is_same_v<T, decl_attr_node> || std::is_same_v<T, decl_var_node>
               || std::is_same_v<T, ident_value_pair_node> || std::is_same_v<T, ident_value_pair_list_node>
               || std::is_same_v<T, entry_node> || std::is_same_v<T, file_node> || std::is_same_v<T, multifile_node>;

template <node T>
auto get_source_location(T const&) -> token::source_location;

} // namespace ast

#include "ast-inl.hpp"
