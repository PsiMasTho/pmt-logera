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