//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/passes/entry_creation_pass.hpp"
#include "logera/errors.hpp"
#include "logera/tokens.hpp"

#include <cassert>

void entry_creation_pass::run()
{
    for (ast::file_node& file : entry_root().children)
    {
        int                    prev_node     = -1;
        token::source_location prev_location = ast::get_source_location(file);
        ast::identifier_node*  cur_ident     = nullptr;
        ast::file_node         modified_file{ .filename = file.filename, .children = {} };
        bool                   value_list_without_ident_reported = false;

        for (auto& child : file.children)
        {
            int                    cur_node = child.index();
            token::source_location cur_location
                = visit([](auto const& n) { return ast::get_source_location(n); }, child);

            if (cur_node != ast::file_node::child_index_v<ast::ident_value_pair_list_node>
                && prev_node == ast::file_node::child_index_v<ast::identifier_node>)
                errors().emplace_back(error::make_record<error::ident_without_value_list>(prev_location));

            visit(
                overloaded{ [&](ast::date_node& n)
                            {
                                cur_ident = nullptr;
                                modified_file.children.push_back(n);
                            },
                            [&](ast::decl_var_node& n)
                            {
                                cur_ident = nullptr;
                                modified_file.children.push_back(std::move(n));
                            },
                            [&](ast::decl_attr_node& n)
                            {
                                cur_ident = nullptr;
                                modified_file.children.push_back(std::move(n));
                            },
                            [&](ast::identifier_node& n) { cur_ident = &n; },
                            [&](ast::ident_value_pair_list_node& n)
                            {
                                if (!cur_ident)
                                {
                                    if (!value_list_without_ident_reported)
                                        errors().emplace_back(
                                            error::make_record<error::value_list_without_ident>(cur_location));
                                    value_list_without_ident_reported = true;
                                }
                                else
                                {
                                    value_list_without_ident_reported = false;
                                    modified_file.children.push_back(
                                        ast::entry_node{ .identifier            = *cur_ident,
                                                         .ident_value_pair_list = std::move(n) });
                                }
                            },
                            [](auto const&) { /*unreachable*/
                                              assert(false);
                            } },
                child);
            prev_node     = cur_node;
            prev_location = cur_location;
        }

        file = std::move(modified_file);
    }
}