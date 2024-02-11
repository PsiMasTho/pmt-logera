#pragma once

#include "tokens.hpp"

#include <string>
#include <variant>
#include <vector>

#include "meta.hpp"

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

    template <typename T> static constexpr auto child_index_v = meta::get_index<T, children_type>::value;

    std::string                                 filename;
    std::vector<children_type>                  children;
};

struct multifile_node
{
    std::vector<file_node> children;
};

template <typename T>
concept decl_node = std::is_same_v<T, decl_attr_node> || std::is_same_v<T, decl_var_node>;

auto get_source_location(multifile_node const&) -> token::source_location;
auto get_source_location(file_node const&) -> token::source_location;
auto get_source_location(entry_node const&) -> token::source_location;
auto get_source_location(ident_value_pair_list_node const&) -> token::source_location;
auto get_source_location(ident_value_pair_node const&) -> token::source_location;
auto get_source_location(decl_var_node const&) -> token::source_location;
auto get_source_location(decl_attr_node const&) -> token::source_location;
auto get_source_location(date_node const&) -> token::source_location;
auto get_source_location(attr_value_node const&) -> token::source_location;
auto get_source_location(identifier_node const&) -> token::source_location;
auto get_source_location(regex_node const&) -> token::source_location;

} // namespace ast
