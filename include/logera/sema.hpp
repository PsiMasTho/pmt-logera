//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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
    split_trees       m_trees;
    error::container& m_errors;

public:
    pass_base(split_trees trees, error::container& errors);
    void operator()();

protected:
    auto entry_root() -> ast::multifile_node&;
    auto decl_attr_root() -> ast::multifile_node&;
    auto decl_var_root() -> ast::multifile_node&;

    auto errors() -> error::container&;
};

template <typename... Ts>
auto apply_passes(split_trees trees, error::container& errors) -> bool;

auto apply_all_passes(split_trees trees, error::container& errors) -> bool;

} // namespace sema

#include "sema-inl.hpp"
