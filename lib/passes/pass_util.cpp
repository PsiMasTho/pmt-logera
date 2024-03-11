//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "pass_util.hpp"

#include <algorithm>

using namespace std;

namespace util
{

auto get_flattened_decls(ast::multifile_node& decl_multifile) -> vector<ast::file_node::children_type*>
{
    vector<ast::file_node::children_type*> flattened_decls;

    for (ast::file_node& file : decl_multifile.children)
    {
        for (auto& decl : file.children)
            if (decl.index() == ast::file_node::child_index_v<ast::decl_attr_node>
                || decl.index() == ast::file_node::child_index_v<ast::decl_var_node>)
                flattened_decls.push_back(&decl);
    }

    sort(
        begin(flattened_decls),
        end(flattened_decls),
        [](auto const& lhs, auto const& rhs)
        { return visit(get_decl_node_ident{}, *lhs) < visit(get_decl_node_ident{}, *rhs); });

    return flattened_decls;
}

} // namespace util