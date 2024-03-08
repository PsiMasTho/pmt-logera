#include "logera/passes/rm_dup_decl_vals_pass.hpp"

#include "logera/algo.hpp"

#include <algorithm>
#include <cassert>

using namespace std;

template <typename T>
struct error_type_selector;

template <>
struct error_type_selector<ast::decl_attr_node>
{
    using type = error::duplicate_attr_value;
};

template <>
struct error_type_selector<ast::decl_var_node>
{
    using type = error::duplicate_var_value;
};

void rm_dup_decl_vals_pass::run()
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

        for (ast::file_node& file : decl_multifile.children)
        {
            for (auto& decl_variant : file.children)
            {
                assert((decl_variant.index() == ast::file_node::child_index_v<T>));

                T& decl = get<T>(decl_variant);

                // sort decl values
                sort(
                    begin(decl.children),
                    end(decl.children),
                    [](auto const& lhs, auto const& rhs)
                    { return to_string_view(lhs.record.lexeme) < to_string_view(rhs.record.lexeme); });

                auto excess_dupes = algo::excess_duplicates(
                    begin(decl.children),
                    end(decl.children),
                    [](auto const& lhs, auto const& rhs)
                    { return to_string_view(lhs.record.lexeme) == to_string_view(rhs.record.lexeme); });

                // report excess dupes
                for (auto const& excess_dupe : excess_dupes)
                {
                    auto const location = ast::get_source_location(*excess_dupe);
                    errors().emplace_back(error::make_record<typename error_type_selector<T>::type>(
                        location,
                        to_string_view(excess_dupe->record.lexeme)));
                }

                // sort excess dupes in reverse by address to allow erasing
                sort(
                    begin(excess_dupes),
                    end(excess_dupes),
                    [](auto const& lhs, auto const& rhs) { return lhs > rhs; });

                // erase excess dupes
                for (auto& excess_dupe : excess_dupes)
                    decl.children.erase(excess_dupe);
            }
        }
    };

    impl.operator()<ast::decl_attr_node>();
    impl.operator()<ast::decl_var_node>();
}