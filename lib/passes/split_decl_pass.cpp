#include "logera/passes/split_decl_pass.hpp"

#include "pass_util.hpp"

#include <algorithm>
#include <cassert>
#include <optional>
#include <type_traits>

using namespace std;

void split_decl_pass::run()
{
    auto const impl = [this]<ast::decl_node T>
    {
        ast::multifile_node& decl_multifile = [this]() -> ast::multifile_node&
        {
            if constexpr (is_same_v<T, ast::decl_attr_node>)
                return decl_attr_root();
            else if constexpr (is_same_v<T, ast::decl_var_node>)
                return decl_var_root();
        }();

        for (auto& file : entry_root().children)
        {
            optional<ast::file_node> decl_file;

            for (auto itr = file.children.begin(); itr != file.children.end();)
            {
                if (itr->index() != ast::file_node::child_index_v<T>)
                {
                    ++itr;
                    continue;
                }

                if (!decl_file)
                    decl_file = ast::file_node{ .filename = file.filename, .children = {} };

                decl_file->children.push_back(std::move(*itr));
                itr = file.children.erase(itr);
            }

            if (!decl_file)
                continue;

            // sort the decls and return them
            sort(
                begin(decl_file->children),
                end(decl_file->children),
                [](auto const& lhs, auto const& rhs)
                {
                    // compare by decl identifier
                    return visit(util::get_decl_node_ident{}, lhs) < visit(util::get_decl_node_ident{}, rhs);
                });

            decl_multifile.children.push_back(std::move(*decl_file));
        }

        if (!decl_multifile.children.empty())
            return;

        if constexpr (is_same_v<T, ast::decl_attr_node>)
            errors().emplace_back(error::code::SEMA_NO_ATTRIBUTES_DECLARED);
        else if constexpr (is_same_v<T, ast::decl_var_node>)
            errors().emplace_back(error::code::SEMA_NO_VARIABLES_DECLARED);
    };

    impl.operator()<ast::decl_attr_node>();
    impl.operator()<ast::decl_var_node>();
}