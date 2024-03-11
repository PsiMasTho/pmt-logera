#include "logera/passes/split_decl_pass.hpp"

#include "logera/ast.hpp"
#include "pass_util.hpp"

#include <algorithm>
#include <cassert>
#include <optional>
#include <type_traits>

using namespace std;

namespace
{

auto is_decl_index(size_t index) -> bool
{
    assert(index < variant_size_v<ast::file_node::children_type>);
    return index == ast::file_node::child_index_v<ast::decl_attr_node>
           || index == ast::file_node::child_index_v<ast::decl_var_node>;
}

auto to_array_index(size_t index) -> size_t
{
    assert(is_decl_index(index));
    return index == ast::file_node::child_index_v<ast::decl_attr_node> ? 0 : 1;
}

} // namespace

void split_decl_pass::run()
{
    ast::multifile_node* decl_multifiles[2] = { &decl_attr_root(), &decl_var_root() };

    for (auto& file : entry_root().children)
    {
        optional<ast::file_node> decl_files[2];
        int                      nondecls = 0, decls = 0;

        for (auto itr = file.children.begin(); itr != file.children.end();)
        {
            if (is_decl_index(itr->index()))
            {
                auto& decl_file = decl_files[to_array_index(itr->index())];
                if (!decl_file)
                    decl_file = ast::file_node{ .filename = file.filename, .children = {} };

                decl_file->children.push_back(std::move(*itr));
                itr = file.children.erase(itr);
                ++decls;
            }
            else
            {
                ++itr;
                ++nondecls;
            }
        }

        if (nondecls > 0 && decls > 0)
            errors().emplace_back(error::make_record<error::mixing_decls_and_nondecls>(file.filename));

        for (int i = 0; i < 2; ++i)
            if (decl_files[i])
                decl_multifiles[i]->children.push_back(std::move(*decl_files[i]));
    }

    if (decl_attr_root().children.empty())
        errors().emplace_back(error::make_record<error::no_attributes_declared>());

    if (decl_var_root().children.empty())
        errors().emplace_back(error::make_record<error::no_variables_declared>());
}