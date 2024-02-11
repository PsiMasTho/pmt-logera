#include "passes.hpp"

#include "algo.hpp"
#include "ast.hpp"
#include "errors.hpp"
#include "levenshtein.hpp"
#include "meta.hpp"
#include "overloaded.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <optional>
#include <type_traits>

using namespace std;

using get_decl_node_ident
    = decltype(overloaded{ [](ast::decl_attr_node const& n) -> char const* { return n.identifier.record.lexeme.data; },
                           [](ast::decl_var_node const& n) -> char const* { return n.identifier.record.lexeme.data; },
                           [](auto const&) -> char const*
                           { /*unreachable*/
                             assert(false);
                             return nullptr;
                           } });

static auto create_entry_node(ast::identifier_node const* ident, ast::ident_value_pair_list_node* value_list) -> ast::entry_node
{
    assert(ident != NULL);
    assert(value_list != NULL);

    // ident must be copied because multiple entries can have the same ident
    return { .identifier = *ident, .ident_value_pair_list = move(*value_list) };
}

// merges duplicate decl values, reports errors
template <typename T> static void pass_3_impl_2(ast::multifile_node* node, std::vector<error::record>& errors)
{
    for (ast::file_node& file : node->children)
    {
        for (auto& decl_variant : file.children)
        {
            assert((decl_variant.index() == meta::get_index<T, decltype(ast::file_node::children)::value_type>::value));

            T& decl = get<T>(decl_variant);

            // sort decl values
            sort(
                begin(decl.children),
                end(decl.children),
                [](auto const& lhs, auto const& rhs) { return strcmp(lhs.record.lexeme.data, rhs.record.lexeme.data) < 0; });

            auto excess_dupes = algo::excess_duplicates(
                begin(decl.children),
                end(decl.children),
                [](auto const& lhs, auto const& rhs) { return strcmp(lhs.record.lexeme.data, rhs.record.lexeme.data) == 0; });

            // report excess dupes
            for (auto const& excess_dupe : excess_dupes)
            {
                auto const location = ast::get_source_location(*excess_dupe);
                errors.emplace_back(
                    is_same_v<T, ast::decl_attr_node> ? error::SEMA_DUPLICATE_ATTR_VALUE : error::SEMA_DUPLICATE_VAR_VALUE,
                    location.filename,
                    location.line,
                    location.column,
                    excess_dupe->record.lexeme.data);
            }

            // erase excess dupes
            for (auto& excess_dupe : excess_dupes)
                decl.children.erase(excess_dupe);
        }
    }
}

// merges duplicate decls, reports errors
template <typename T> static void pass_3_impl_1(ast::multifile_node* node, std::vector<error::record>& errors)
{
    std::vector<decltype(ast::file_node::children)::value_type*> flattened_decls;

    for (ast::file_node& file : node->children)
        for (auto& decl : file.children)
            flattened_decls.push_back(&decl);

    sort(
        begin(flattened_decls),
        end(flattened_decls),
        [](auto const& lhs, auto const& rhs)
        { return strcmp(visit(get_decl_node_ident{}, *lhs), visit(get_decl_node_ident{}, *rhs)) < 0; });

    auto excess_dupes = algo::excess_duplicates_v(
        begin(flattened_decls),
        end(flattened_decls),
        [](auto const& lhs, auto const& rhs)
        { return strcmp(visit(get_decl_node_ident{}, *lhs), visit(get_decl_node_ident{}, *rhs)) == 0; });

    // report excess dupes
    for (auto const& excess_dupe : excess_dupes)
    {
        auto const location = ast::get_source_location(get<T>(*excess_dupe));
        errors.emplace_back(
            is_same_v<T, ast::decl_attr_node> ? error::SEMA_DUPLICATE_ATTR : error::SEMA_DUPLICATE_VAR,
            location.filename,
            location.line,
            location.column,
            (get<T>(*excess_dupe)).identifier.record.lexeme.data);
    }

    auto dupes = algo::duplicates_v(
        begin(flattened_decls),
        end(flattened_decls),
        [](auto const& lhs, auto const& rhs)
        {
            // compare by decl identifier
            return strcmp(visit(get_decl_node_ident{}, *lhs), visit(get_decl_node_ident{}, *rhs)) == 0;
        });

    // merge non-name children into the first duplicate
    for (size_t i = 0; i < dupes.size(); ++i)
    {
        auto* const dupe = dupes[i];

        while (i + 1 < dupes.size())
        {
            auto* const lookahead = dupes[i + 1];

            if (strcmp(get<T>(*dupe).identifier.record.lexeme.data, get<T>(*lookahead).identifier.record.lexeme.data) != 0)
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
    for (ast::file_node& file : node->children)
        for (auto& excess_dupe : excess_dupes)
            if (excess_dupe >= &file.children.front() && excess_dupe <= &file.children.back())
                file.children.erase(file.children.begin() + distance(&file.children.front(), excess_dupe));
}

namespace sema
{

auto is_valid_date(char const* str) -> bool
{
    assert(str != NULL);
    assert(strlen(str) == 10);

    int       y, m, d;
    int const matched = sscanf(str, "%d-%d-%d", &y, &m, &d);

    assert(matched == 3);

    int mon_day[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (y < 1 || m < 1 || m > 12)
        return false;

    bool const leap = (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0));

    mon_day[1] += leap;

    if ((d > mon_day[m - 1]) || (d < 1))
        return false;

    return true;
}

checker::checker(ast::multifile_node&& mf_node, vector<error::record>& errors)
    : m_entry_root(move(mf_node))
    , m_decl_attr_root{}
    , m_decl_var_root{}
    , m_errors{ errors }
{
}

auto checker::check() -> bool
{
    size_t const initial_errors = m_errors.size();

    pass_1();
    pass_2();
    pass_3();
    pass_4();
    pass_5();

    return m_errors.size() == initial_errors;
}

void checker::pass_1()
{
    for (ast::file_node& file : m_entry_root.children)
    {
        int                    prev_node     = -1;
        token::source_location prev_location = ast::get_source_location(file);
        ast::identifier_node*  cur_ident     = nullptr;
        ast::file_node         modified_file{ .filename = file.filename, .children = {} };
        bool                   value_list_without_ident_reported = false;

        for (auto& child : file.children)
        {
            int                    cur_node     = child.index();
            token::source_location cur_location = visit([](auto const& n) { return ast::get_source_location(n); }, child);

            if (cur_node != ast::file_node::child_index_v<ast::ident_value_pair_list_node>
                && prev_node == ast::file_node::child_index_v<ast::identifier_node>)
                m_errors.emplace_back(
                    error::code::SEMA_IDENT_WITHOUT_VALUE_LIST,
                    prev_location.filename,
                    prev_location.line,
                    prev_location.column);

            visit(
                overloaded{ [&](ast::date_node& n)
                            {
                                cur_ident = nullptr;
                                modified_file.children.push_back(std::move(n));
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
                                        m_errors.emplace_back(
                                            error::code::SEMA_VALUE_LIST_WITHOUT_IDENT,
                                            cur_location.filename,
                                            cur_location.line,
                                            cur_location.column);
                                    value_list_without_ident_reported = true;
                                }
                                else
                                {
                                    value_list_without_ident_reported = false;
                                    modified_file.children.push_back(create_entry_node(cur_ident, &n));
                                }
                            },
                            [](auto const&) { /*unreachable*/
                                              assert(false);
                            } },
                child);
            prev_node     = cur_node;
            prev_location = cur_location;
        }

        file = move(modified_file);
    }
}

auto checker::pass_2()
{
    pair<ast::multifile_node, ast::multifile_node> ret;

    for (auto& file : node->children)
    {
        optional<ast::file_node> ret_files[2];
        ast::multifile_node*     out_multifiles[2] = { &ret.first, &ret.second };

        for (auto itr = file.children.begin(); itr != file.children.end();)
        {
            if (itr->index() != meta::get_index<ast::decl_attr_node, decltype(ast::file_node::children)::value_type>::value
                && itr->index() != meta::get_index<ast::decl_var_node, decltype(ast::file_node::children)::value_type>::value)
            {
                ++itr;
                continue;
            }

            int const index = itr->index() == meta::get_index<ast::decl_var_node, decltype(ast::file_node::children)::value_type>::value;

            if (!ret_files[index])
                ret_files[index] = ast::file_node{ .filename = file.filename, .children = {} };

            ret_files[index]->children.push_back(std::move(*itr));
            itr = file.children.erase(itr);
        }

        // sort the decls and return them
        for (int i = 0; i < 2; ++i)
        {
            if (!ret_files[i])
                continue;

            sort(
                ret_files[i]->children.begin(),
                ret_files[i]->children.end(),
                [](auto const& lhs, auto const& rhs)
                {
                    // compare by decl identifier
                    return strcmp(visit(get_decl_node_ident{}, lhs), visit(get_decl_node_ident{}, rhs)) < 0;
                });

            out_multifiles[i]->children.push_back(std::move(*ret_files[i]));
        }
    }

    if (ret.first.children.empty())
        errors.emplace_back(error::code::SEMA_NO_ATTRIBUTES_DECLARED);
    if (ret.second.children.empty())
        errors.emplace_back(error::code::SEMA_NO_VARIABLES_DECLARED);

    return ret;
}

void pass_3(std::pair<ast::multifile_node, ast::multifile_node>& decls, std::vector<error::record>& errors)
{
    if (!decls.first.children.empty())
    {
        pass_3_impl_1<ast::decl_attr_node>(&decls.first, errors);
        pass_3_impl_2<ast::decl_attr_node>(&decls.first, errors);
    }

    if (!decls.second.children.empty())
    {
        pass_3_impl_1<ast::decl_var_node>(&decls.second, errors);
        pass_3_impl_2<ast::decl_var_node>(&decls.second, errors);
    }
}

void pass_4(std::pair<ast::multifile_node, ast::multifile_node>& decls, std::vector<error::record>& errors)
{
    for (ast::file_node& var_file : decls.second.children)
    {
        for (size_t i = 0; i < var_file.children.size(); ++i)
        {
            auto& var_decl = var_file.children[i];
            for (size_t j = 0; j < get<ast::decl_var_node>(var_decl).children.size(); ++j)
            {
                bool const found = any_of(
                    decls.first.children.begin(),
                    decls.first.children.end(),
                    [j, &var_decl](auto const& attr_file)
                    {
                        return binary_search(
                            attr_file.children.begin(),
                            attr_file.children.end(),
                            var_decl,
                            [j](auto const& lhs, auto const& rhs)
                            {
                                assert(lhs.index() != rhs.index());
                                if (auto const* l = get_if<ast::decl_attr_node>(&lhs))
                                    return strcmp(
                                               l->identifier.record.lexeme.data,
                                               get<ast::decl_var_node>(rhs).children[j].record.lexeme.data)
                                           < 0;
                                else if (auto const* l = get_if<ast::decl_var_node>(&lhs))
                                    return strcmp(
                                               l->children[j].record.lexeme.data,
                                               get<ast::decl_attr_node>(rhs).identifier.record.lexeme.data)
                                           < 0;
                                else
                                    assert(false);
                            });
                    });

                if (found)
                    continue;

                // find smallest levenshtein distance
                min_element

                    // if found
                    // skip
                    // if not found
                    //  levenshtein small
                    //  report hinted error
                    // levenshtein large
                    // report undeclared error

                    if (attr == NULL)
                {
                    push_fmted_error(
                        error::SEMA_UNDECLARED_ATTR,
                        pass_error_fmts,
                        errors,
                        ast_node_get_source_location(value),
                        value->tok.lexeme,
                        var_name);
                    ast_node_destroy(value);
                    *value = ast_node_create(EMPTY_NODE);
                }
            }
            opaque_vector_erase_if(&decl->children, is_empty_node);
        }
    }
}

void checker::pass_5()
{
    /*
                                    ++n_dates_encountered;
                                    if (prev_node != -1)
                                        m_errors.emplace_back(
                                            error::code::SEMA_DATE_NOT_FIRST_IN_FILE,
                                            cur_location.filename,
                                            cur_location.line,
                                            cur_location.column);
                                    if (!is_valid_date(n.record.lexeme.data))
                                        m_errors.emplace_back(
                                            error::code::SEMA_INVALID_DATE,
                                            cur_location.filename,
                                            cur_location.line,
                                            cur_location.column,
                                            n.record.lexeme.data);
                                    if (n_dates_encountered > 1)
                                    {
                                        m_errors.emplace_back(
                                            error::code::SEMA_MULTIPLE_DATES,
                                            cur_location.filename,
                                            cur_location.line,
                                            cur_location.column);
                                        return;
                                    }
    */

    /*
                                [&](ast::identifier_node& n)
                                {
                                    if (n_dates_encountered == 0)
                                        errors.emplace_back(
                                            error::code::SEMA_NO_DATE_BEFORE_ENTRY,
                                            cur_location.filename,
                                            cur_location.line,
                                            cur_location.column);
                                    cur_ident = &n;
                                },
    */
}

#if 0
opaque_vector create_regex_matchers(ast_node const* decl_attrs, opaque_vector* errors)
{
    assert(decl_attrs != NULL);
    assert(decl_attrs->type == MULTIFILE_NODE);
    assert(errors != NULL);
}

int cmp_attr_data_by_name(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
}

bool check_regex_match(
    ast_node const*      decl_attr_root,
    opaque_vector const* matchers,
    ast_node const*      attr_node,
    ast_node const*      value_node,
    opaque_vector*       errors)
{
    assert(decl_attr_root != NULL);
    assert(decl_attr_root->type == MULTIFILE_NODE);
    assert(matchers != NULL);
    assert(attr_node != NULL);
    assert(attr_node->type == IDENTIFIER_NODE);
    assert(value_node != NULL);
    assert(value_node->type == ATTR_VALUE_NODE);
    assert(errors != NULL);
}

#endif

} // namespace sema
