#pragma once

#include "ast.hpp"
#include "errors.hpp"

#include <array>
#include <regex>
#include <tuple>
#include <utility>
#include <vector>

namespace sema
{

enum
{
    ENTRY_ROOT_INDEX = 0,
    DECL_ATTR_ROOT_INDEX,
    DECL_VAR_ROOT_INDEX
};

enum
{
    SIMILAR_LEV = 3 // The maximum Levenshtein distance between two strings for them to be considered similar.
};

using split_trees = std::tuple<ast::multifile_node*, ast::multifile_node*, ast::multifile_node*>;

template <typename Derived>
class pass_base
{
    split_trees                 m_trees;
    std::vector<error::record>& m_errors;

public:
    pass_base(split_trees trees, std::vector<error::record>& errors);
    void operator()();

protected:
    auto entry_root() -> ast::multifile_node&;
    auto decl_attr_root() -> ast::multifile_node&;
    auto decl_var_root() -> ast::multifile_node&;

    auto errors() -> std::vector<error::record>&;
};

template <typename... Ts>
auto apply_passes(split_trees trees, std::vector<error::record>& errors) -> bool;

auto apply_all_passes(split_trees trees, std::vector<error::record>& errors) -> bool;

} // namespace sema

#include "sema-inl.hpp"
