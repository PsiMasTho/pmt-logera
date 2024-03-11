//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "logera/ast.hpp"
#include "logera/overloaded.hpp"

#include <cassert>
#include <string_view>
#include <vector>

namespace util
{

using get_decl_node_ident = decltype(overloaded{
    [](ast::decl_attr_node const& n) -> std::string_view { return to_string_view(n.identifier.record.lexeme); },
    [](ast::decl_var_node const& n) -> std::string_view { return to_string_view(n.identifier.record.lexeme); },
    [](auto const&) -> std::string_view { /*unreachable*/
                                          assert(false);
                                          return {};
    } });

auto get_flattened_decls(ast::multifile_node& decl_multifile) -> std::vector<ast::file_node::children_type*>;

} // namespace util