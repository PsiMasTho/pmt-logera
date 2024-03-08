#include "logera/passes/decl_var_val_check_pass.hpp"

#include "logera/levenshtein.hpp"
#include "pass_util.hpp"

#include <algorithm>

using namespace std;

void decl_var_val_check_pass::run()
{
    auto flattened_decl_attrs = util::get_flattened_decls(decl_attr_root());
    auto flattened_decl_vars  = util::get_flattened_decls(decl_var_root());

    levenshtein lev;
    vector<int> distances;

    for (auto const& var_decl : flattened_decl_vars)
    {
        for (size_t i = 0; i < get<ast::decl_var_node>(*var_decl).children.size();)
        {
            bool const found = any_of(
                begin(decl_attr_root().children),
                end(decl_attr_root().children),
                [i, &var_decl](auto const& attr_file)
                {
                    return binary_search(
                        attr_file.children.begin(),
                        attr_file.children.end(),
                        *var_decl,
                        [i](auto const& lhs, auto const& rhs)
                        {
                            assert(lhs.index() != rhs.index());
                            if (auto const* l = get_if<ast::decl_attr_node>(&lhs))
                            {
                                return to_string_view(l->identifier.record.lexeme)
                                       < to_string_view(get<ast::decl_var_node>(rhs).children[i].record.lexeme);
                            }
                            return to_string_view(get<ast::decl_var_node>(lhs).children[i].record.lexeme)
                                   < to_string_view(get<ast::decl_attr_node>(rhs).identifier.record.lexeme);
                        });
                });

            if (found)
            {
                ++i;
                continue;
            }

            // find smallest levenshtein distance
            distances.clear();
            distances.resize(flattened_decl_attrs.size());

            transform(
                begin(flattened_decl_attrs),
                end(flattened_decl_attrs),
                begin(distances),
                [i, &var_decl, &lev](auto const& attr_decl)
                {
                    return lev.distance(
                        to_string_view(get<ast::decl_var_node>(*var_decl).children[i].record.lexeme),
                        to_string_view(get<ast::decl_attr_node>(*attr_decl).identifier.record.lexeme));
                });

            auto const min_lev = min_element(begin(distances), end(distances));

            auto const loc = ast::get_source_location(get<ast::decl_var_node>(*var_decl).children[i]);

            if (*min_lev > sema::SIMILAR_LEV)
            {
                errors().emplace_back(error::make_record<error::undeclared_attr_in_var_decl>(
                    loc,
                    get<ast::decl_var_node>(*var_decl).children[i].record.lexeme.data(),
                    get<ast::decl_var_node>(*var_decl).identifier.record.lexeme.data()));
            }
            else
            {
                errors().emplace_back(error::make_record<error::undeclared_attr_in_var_decl_w_hint>(
                    loc,
                    get<ast::decl_var_node>(*var_decl).children[i].record.lexeme.data(),
                    get<ast::decl_var_node>(*var_decl).identifier.record.lexeme.data(),
                    get<ast::decl_attr_node>(*flattened_decl_attrs[distance(begin(distances), min_lev)])
                        .identifier.record.lexeme.data()));
            }

            // remove invalid value
            get<ast::decl_var_node>(*var_decl).children.erase(get<ast::decl_var_node>(*var_decl).children.begin() + i);
        }
    }
}