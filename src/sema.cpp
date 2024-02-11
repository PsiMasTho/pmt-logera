#include "sema.hpp"

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

static auto
create_entry_node(ast::identifier_node const* ident, ast::ident_value_pair_list_node* value_list) -> ast::entry_node
{
    assert(ident != NULL);
    assert(value_list != NULL);

    // ident must be copied because multiple entries can have the same ident
    return { .identifier = *ident, .ident_value_pair_list = std::move(*value_list) };
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
    : m_entry_root(std::move(mf_node))
    , m_decl_roots{}
    , m_errors{ errors }
{
}

auto checker::get_entry_root() -> ast::multifile_node&
{
    return m_entry_root;
}

auto checker::get_decl_attr_root() -> ast::multifile_node&
{
    return m_decl_roots[decl_root_index<ast::decl_attr_node>];
}

auto checker::get_decl_var_root() -> ast::multifile_node&
{
    return m_decl_roots[decl_root_index<ast::decl_var_node>];
}

auto checker::check() -> bool
{
    size_t const initial_errors = m_errors.size();

    pass_1();
    pass_2();
    pass_3();
    pass_4();
    pass_5();
    pass_6();
    pass_7();

    return m_errors.size() == initial_errors;
}

void checker::pass_1()
{
    sort(
        begin(m_entry_root.children),
        end(m_entry_root.children),
        [](auto const& lhs, auto const& rhs) { return lhs.filename < rhs.filename; });

    auto excess_dupes = algo::excess_duplicates(
        begin(m_entry_root.children),
        end(m_entry_root.children),
        [](auto const& lhs, auto const& rhs) { return lhs.filename == rhs.filename; });

    for (auto const& excess_dupe : excess_dupes)
        m_errors.emplace_back(error::code::SEMA_DUPLICATE_FILENAME, excess_dupe->filename.c_str());

    // erase excess dupes from ast
    sort(begin(excess_dupes), end(excess_dupes), [](auto const& lhs, auto const& rhs) { return lhs > rhs; });

    for (auto const& excess_dupe : excess_dupes)
        m_entry_root.children.erase(excess_dupe);
}

void checker::pass_2()
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
            int                    cur_node = child.index();
            token::source_location cur_location
                = visit([](auto const& n) { return ast::get_source_location(n); }, child);

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

        file = std::move(modified_file);
    }
}

void checker::pass_3()
{
    auto const impl = [this]<ast::decl_node T>
    {
        ast::multifile_node& decl_multifile = m_decl_roots[decl_root_index<T>];
        for (auto& file : m_entry_root.children)
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
                    return strcmp(visit(get_decl_node_ident{}, lhs), visit(get_decl_node_ident{}, rhs)) < 0;
                });

            decl_multifile.children.push_back(std::move(*decl_file));
        }

        if (!decl_multifile.children.empty())
            return;

        if constexpr (is_same_v<T, ast::decl_attr_node>)
            m_errors.emplace_back(error::code::SEMA_NO_ATTRIBUTES_DECLARED);
        else if constexpr (is_same_v<T, ast::decl_var_node>)
            m_errors.emplace_back(error::code::SEMA_NO_VARIABLES_DECLARED);
    };

    impl.operator()<ast::decl_attr_node>();
    impl.operator()<ast::decl_var_node>();
}

void checker::pass_4()
{
    auto const impl = [this]<ast::decl_node T>
    {
        ast::multifile_node& decl_multifile = m_decl_roots[decl_root_index<T>];
        if (decl_multifile.children.empty())
            return;

        std::vector<ast::file_node::children_type*> flattened_decls;

        for (ast::file_node& file : decl_multifile.children)
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
            m_errors.emplace_back(
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

                if (strcmp(
                        get<T>(*dupe).identifier.record.lexeme.data,
                        get<T>(*lookahead).identifier.record.lexeme.data)
                    != 0)
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

void checker::pass_5()
{
    auto const impl = [this]<ast::decl_node T>
    {
        ast::multifile_node& decl_multifile = m_decl_roots[decl_root_index<T>];

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
                    { return strcmp(lhs.record.lexeme.data, rhs.record.lexeme.data) < 0; });

                auto excess_dupes = algo::excess_duplicates(
                    begin(decl.children),
                    end(decl.children),
                    [](auto const& lhs, auto const& rhs)
                    { return strcmp(lhs.record.lexeme.data, rhs.record.lexeme.data) == 0; });

                // report excess dupes
                for (auto const& excess_dupe : excess_dupes)
                {
                    auto const location = ast::get_source_location(*excess_dupe);
                    m_errors.emplace_back(
                        is_same_v<T, ast::decl_attr_node> ? error::SEMA_DUPLICATE_ATTR_VALUE
                                                          : error::SEMA_DUPLICATE_VAR_VALUE,
                        location.filename,
                        location.line,
                        location.column,
                        excess_dupe->record.lexeme.data);
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

void checker::pass_6()
{
    vector<ast::file_node::children_type*> flattened_decl_attrs;
    for (ast::file_node& file : m_decl_roots[decl_root_index<ast::decl_attr_node>].children)
        for (auto& decl : file.children)
            flattened_decl_attrs.push_back(&decl);

    sort(
        begin(flattened_decl_attrs),
        end(flattened_decl_attrs),
        [](auto const& lhs, auto const& rhs)
        {
            return strcmp(
                       get<ast::decl_attr_node>(*lhs).identifier.record.lexeme.data,
                       get<ast::decl_attr_node>(*rhs).identifier.record.lexeme.data)
                   < 0;
        });

    vector<ast::file_node::children_type*> flattened_decl_vars;
    for (ast::file_node& file : m_decl_roots[decl_root_index<ast::decl_var_node>].children)
        for (auto& decl : file.children)
            flattened_decl_vars.push_back(&decl);

    levenshtein lev;
    vector<int> distances;

    for (auto const& var_decl : flattened_decl_vars)
    {
        for (size_t i = 0; i < get<ast::decl_var_node>(*var_decl).children.size();)
        {
            bool const found = any_of(
                begin(m_decl_roots[decl_root_index<ast::decl_attr_node>].children),
                end(m_decl_roots[decl_root_index<ast::decl_attr_node>].children),
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
                                return strcmp(
                                           l->identifier.record.lexeme.data,
                                           get<ast::decl_var_node>(rhs).children[i].record.lexeme.data)
                                       < 0;
                            }
                            return strcmp(
                                       get<ast::decl_var_node>(lhs).children[i].record.lexeme.data,
                                       get<ast::decl_attr_node>(rhs).identifier.record.lexeme.data)
                                   < 0;
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
                        get<ast::decl_var_node>(*var_decl).children[i].record.lexeme.data,
                        get<ast::decl_attr_node>(*attr_decl).identifier.record.lexeme.data);
                });

            auto const min_lev = min_element(begin(distances), end(distances));

            auto const loc = ast::get_source_location(get<ast::decl_var_node>(*var_decl).children[i]);

            if (*min_lev > 3)
            {
                m_errors.emplace_back(
                    error::code::SEMA_UNDECLARED_ATTR,
                    loc.filename,
                    loc.line,
                    loc.column,
                    get<ast::decl_var_node>(*var_decl).children[i].record.lexeme.data,
                    get<ast::decl_var_node>(*var_decl).identifier.record.lexeme.data);
            }
            else
            {
                m_errors.emplace_back(
                    error::code::SEMA_UNDECLARED_ATTR_W_HINT,
                    loc.filename,
                    loc.line,
                    loc.column,
                    get<ast::decl_var_node>(*var_decl).children[i].record.lexeme.data,
                    get<ast::decl_var_node>(*var_decl).identifier.record.lexeme.data,
                    get<ast::decl_attr_node>(*flattened_decl_attrs[distance(begin(distances), min_lev)])
                        .identifier.record.lexeme.data);
            }

            // remove invalid value
            get<ast::decl_var_node>(*var_decl).children.erase(get<ast::decl_var_node>(*var_decl).children.begin() + i);
        }
    }
}

void checker::pass_7()
{
    flyweight_string* prev_date = nullptr;

    for (ast::file_node& file : m_entry_root.children)
    {
        int                                    n_dates_encountered = 0;
        vector<ast::file_node::children_type*> excess_dates;
        int                                    prev_node                           = -1;
        bool                                   date_not_in_filename_order_reported = false;
        bool                                   no_date_before_entry_reported       = false;
        for (auto& child : file.children)
        {
            int const  cur_node     = child.index();
            auto const cur_location = visit([](auto const& n) { return ast::get_source_location(n); }, child);

            visit(
                overloaded{ [&](ast::date_node& n)
                            {
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
                                if (prev_date && !date_not_in_filename_order_reported)
                                {
                                    if (strcmp(prev_date->data, n.record.lexeme.data) > 0)
                                    {
                                        m_errors.emplace_back(
                                            error::code::SEMA_DATE_NOT_IN_FILENAME_ORDER,
                                            cur_location.filename,
                                            cur_location.line,
                                            cur_location.column);
                                        date_not_in_filename_order_reported = true;
                                    }
                                }
                                if (n_dates_encountered > 1)
                                {
                                    m_errors.emplace_back(
                                        error::code::SEMA_MULTIPLE_DATES,
                                        cur_location.filename,
                                        cur_location.line,
                                        cur_location.column);

                                    excess_dates.push_back(&child);

                                    return;
                                }
                                prev_date = &n.record.lexeme;
                            },
                            [&](ast::entry_node& n)
                            {
                                if (n_dates_encountered == 0 && !no_date_before_entry_reported)
                                {
                                    m_errors.emplace_back(
                                        error::code::SEMA_NO_DATE_BEFORE_ENTRY,
                                        ast::get_source_location(n).filename,
                                        ast::get_source_location(n).line,
                                        ast::get_source_location(n).column);
                                    no_date_before_entry_reported = true;
                                }
                            },
                            [&](auto const&) { /*unreachable*/
                                               assert(false);
                            } },
                child);
            prev_node = cur_node;
        }
        // erase excess dates
        sort(begin(excess_dates), end(excess_dates), [](auto const& lhs, auto const& rhs) { return lhs > rhs; });

        for (auto& excess_date : excess_dates)
            file.children.erase(file.children.begin() + distance(&file.children.front(), excess_date));
    }
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
