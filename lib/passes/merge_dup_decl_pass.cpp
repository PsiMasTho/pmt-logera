//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/passes/merge_dup_decl_pass.hpp"

#include "logera/algo.hpp"
#include "pass_util.hpp"

using namespace std;

template <typename T>
struct error_type_selector;

template <>
struct error_type_selector<ast::decl_attr_node>
{
    using type = error::duplicate_attr_decl;
};

template <>
struct error_type_selector<ast::decl_var_node>
{
    using type = error::duplicate_var_decl;
};

void merge_dup_decl_pass::run()
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

        if (decl_multifile.children.empty())
            return;

        auto flattened_decls = util::get_flattened_decls(decl_multifile);

        auto excess_dupes = algo::excess_duplicates_v(
            begin(flattened_decls),
            end(flattened_decls),
            [](auto const& lhs, auto const& rhs)
            { return visit(util::get_decl_node_ident{}, *lhs) == visit(util::get_decl_node_ident{}, *rhs); });

        // report excess dupes
        for (auto const& excess_dupe : excess_dupes)
        {
            auto const location = ast::get_source_location(get<T>(*excess_dupe));
            errors().emplace_back(error::make_record<typename error_type_selector<T>::type>(
                location,
                to_string_view(get<T>(*excess_dupe).identifier.record.lexeme)));
        }

        auto dupes = algo::duplicates_v(
            begin(flattened_decls),
            end(flattened_decls),
            [](auto const& lhs, auto const& rhs)
            { return visit(util::get_decl_node_ident{}, *lhs) == visit(util::get_decl_node_ident{}, *rhs); });

        // merge non-name children into the first duplicate
        for (size_t i = 0; i < dupes.size(); ++i)
        {
            auto* const dupe = dupes[i];

            while (i + 1 < dupes.size())
            {
                auto* const lookahead = dupes[i + 1];

                if (to_string_view(get<T>(*dupe).identifier.record.lexeme)
                    != to_string_view(get<T>(*lookahead).identifier.record.lexeme))
                    break;

                // merge lookahead children into dupe
                for (auto& child : get<T>(*lookahead).children)
                    get<T>(*dupe).children.push_back(std::move(child));

                // erase lookahead node from dupes
                dupes.erase(dupes.begin() + i + 1);
            }
        }

        // sort excess dupes in reverse by address to allow erasing
        sort(begin(excess_dupes), end(excess_dupes), [](auto const& lhs, auto const& rhs) { return lhs > rhs; });

        // erase excess dupes from ast
        for (ast::file_node& file : decl_multifile.children)
            for (auto& excess_dupe : excess_dupes)
                if (excess_dupe >= &file.children.front() && excess_dupe <= &file.children.back())
                    file.children.erase(file.children.begin() + distance(&file.children.front(), excess_dupe));
    };

    impl.operator()<ast::decl_attr_node>();
    impl.operator()<ast::decl_var_node>();
}