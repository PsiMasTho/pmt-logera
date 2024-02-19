#include "logera/passes/entry_validation_pass.hpp"

#include "pass_util.hpp"

#include <algorithm>

using namespace std;

void attr_matcher::add_regex(char const* expr)
{
    m_regexes.emplace_back(expr, regex::optimize);
}

auto attr_matcher::operator()(char const* str) const -> bool
{
    // if no regex is defined, then assume it's a match
    if (m_regexes.empty())
        return true;

    // does any regex match?
    return any_of(begin(m_regexes), end(m_regexes), [&str](auto const& rexpr) { return regex_match(str, rexpr); });
}

void entry_validation_pass::run()
{
    construct_flattened_decls();
    construct_matchers();

    for (ast::file_node& file : entry_root().children)
    {
        ast::identifier_node* prev_entry_ident = nullptr;
        for (size_t i = 0; i < file.children.size(); ++i)
        {
            ast::entry_node* entry = get_if<ast::entry_node>(&file.children[i]);

            if (!entry)
                continue;

            ast::identifier_node* cur_entry_ident = &entry->identifier;

            if (!is_entry_variable_declared(*entry))
            {
                if (prev_entry_ident
                    && to_string_view(prev_entry_ident->record.lexeme)
                           != to_string_view(cur_entry_ident->record.lexeme))
                    report_undeclared_var_in_entry(*entry);

                // process IVPL
                for (auto const& ivp : entry->ident_value_pair_list.children)
                {
                    if (auto* matcher = is_attr_declared_globally(ivp.identifier))
                    {
                        if (matcher->operator()(ivp.attr_value.record.lexeme.data()))
                            continue;
                        report_regex_mismatch(ivp);
                    }
                    else
                        report_undeclared_attr_globally(ivp.identifier);
                }
            }
            else
            {
                // process IVPL
                for (auto const& ivp : entry->ident_value_pair_list.children)
                {
                    if (!is_attr_declared_for_var(ivp.identifier, entry->identifier))
                        report_undeclared_attr_for_var(ivp.identifier, entry->identifier);

                    if (auto* matcher = is_attr_declared_globally(ivp.identifier))
                    {
                        if (matcher->operator()(ivp.attr_value.record.lexeme.data()))
                            continue;
                        report_regex_mismatch(ivp);
                    }
                    else
                        report_undeclared_attr_globally(ivp.identifier);
                }
            }
            prev_entry_ident = cur_entry_ident;
        }
    }
}

void entry_validation_pass::construct_flattened_decls()
{
    m_flat_attrs = util::get_flattened_decls(decl_attr_root());
    m_flat_vars  = util::get_flattened_decls(decl_var_root());
}

void entry_validation_pass::construct_matchers()
{
    m_matchers.reserve(m_flat_attrs.size());

    for (auto const* attr : m_flat_attrs)
    {
        m_matchers.emplace_back();
        for (auto const& value : get<ast::decl_attr_node>(*attr).children)
        {
            try
            {
                m_matchers.back().add_regex(value.record.lexeme.data());
            }
            catch (regex_error const& e)
            {
                auto const location = ast::get_source_location(value);
                errors().emplace_back(
                    error::code::SEMA_REGCOMP_FAILED,
                    location.filename,
                    location.line,
                    location.column,
                    value.record.lexeme.data(),
                    get<ast::decl_attr_node>(*attr).identifier.record.lexeme.data(),
                    e.what());
            }
            // let other exceptions propagate
        }
    }
}

auto entry_validation_pass::is_entry_variable_declared(ast::entry_node const& entry) const -> bool
{
    return binary_search(
        begin(m_flat_vars),
        end(m_flat_vars),
        to_string_view(entry.identifier.record.lexeme),
        overloaded{ [](string_view const& lhs, ast::file_node::children_type* rhs)
                    { return lhs < to_string_view(get<ast::decl_var_node>(*rhs).identifier.record.lexeme); },
                    [](ast::file_node::children_type* lhs, string_view const& rhs)
                    { return to_string_view(get<ast::decl_var_node>(*lhs).identifier.record.lexeme) < rhs; } });
}

void entry_validation_pass::report_undeclared_var_in_entry(ast::entry_node const& entry)
{
    m_lev_distances.resize(m_flat_vars.size());

    transform(
        m_flat_vars.begin(),
        m_flat_vars.end(),
        m_lev_distances.begin(),
        [&entry, this](auto const& var_decl)
        {
            return m_lev.distance(
                to_string_view(entry.identifier.record.lexeme),
                to_string_view(get<ast::decl_var_node>(*var_decl).identifier.record.lexeme));
        });

    auto const min_lev = min_element(m_lev_distances.begin(), m_lev_distances.end());

    auto const location = ast::get_source_location(entry.identifier);

    if (*min_lev > sema::SIMILAR_LEV)
    {
        errors().emplace_back(
            error::code::SEMA_UNDECLARED_VAR_IN_ENTRY,
            location.filename,
            location.line,
            location.column,
            entry.identifier.record.lexeme.data());
    }
    else
    {
        errors().emplace_back(
            error::code::SEMA_UNDECLARED_VAR_IN_ENTRY_W_HINT,
            location.filename,
            location.line,
            location.column,
            entry.identifier.record.lexeme.data(),
            get<ast::decl_var_node>(*m_flat_vars[distance(m_lev_distances.begin(), min_lev)])
                .identifier.record.lexeme.data());
    }
}

void entry_validation_pass::report_undeclared_attr_for_var(
    ast::identifier_node const& attr,
    ast::identifier_node const& var)
{
    // find the variable declaration
    auto itr = lower_bound(
        m_flat_vars.begin(),
        m_flat_vars.end(),
        to_string_view(var.record.lexeme),
        overloaded{
            [](string_view const& lhs, ast::file_node::children_type* rhs)
            { return lhs < to_string_view(get<ast::decl_var_node>(*rhs).identifier.record.lexeme); },
            [](ast::file_node::children_type* lhs, string_view const& rhs)
            { return to_string_view(get<ast::decl_var_node>(*lhs).identifier.record.lexeme) < rhs; },
        });

    if (itr == m_flat_vars.end()
        || to_string_view(get<ast::decl_var_node>(**itr).identifier.record.lexeme) != to_string_view(var.record.lexeme))
        assert(0); // unreachable

    auto const& var_decl = get<ast::decl_var_node>(**itr);

    m_lev_distances.resize(var_decl.children.size());

    transform(
        var_decl.children.begin(),
        var_decl.children.end(),
        m_lev_distances.begin(),
        [&attr, this](auto const& attr_decl)
        { return m_lev.distance(to_string_view(attr.record.lexeme), to_string_view(attr_decl.record.lexeme)); });

    auto const min_lev = min_element(m_lev_distances.begin(), m_lev_distances.end());

    auto const location = ast::get_source_location(attr);

    if (*min_lev > sema::SIMILAR_LEV)
    {
        errors().emplace_back(
            error::code::SEMA_UNDECLARED_ATTR_IN_ENTRY_LOCAL,
            location.filename,
            location.line,
            location.column,
            attr.record.lexeme.data(),
            var.record.lexeme.data());
    }
    else
    {
        errors().emplace_back(
            error::code::SEMA_UNDECLARED_ATTR_IN_ENTRY_LOCAL_W_HINT,
            location.filename,
            location.line,
            location.column,
            attr.record.lexeme.data(),
            var.record.lexeme.data(),
            var_decl.children[distance(m_lev_distances.begin(), min_lev)].record.lexeme.data());
    }
}

void entry_validation_pass::report_regex_mismatch(ast::ident_value_pair_node const& ivp)
{
    auto const location = ast::get_source_location(ivp);

    errors().emplace_back(
        error::code::SEMA_REGEX_MISMATCH,
        location.filename,
        location.line,
        location.column,
        ivp.attr_value.record.lexeme.data(),
        ivp.identifier.record.lexeme.data());
}

void entry_validation_pass::report_undeclared_attr_globally(ast::identifier_node const& attr)
{
    m_lev_distances.resize(m_flat_attrs.size());

    transform(
        m_flat_attrs.begin(),
        m_flat_attrs.end(),
        m_lev_distances.begin(),
        [&attr, this](auto const& attr_decl)
        {
            return m_lev.distance(
                to_string_view(attr.record.lexeme),
                to_string_view(get<ast::decl_attr_node>(*attr_decl).identifier.record.lexeme));
        });

    auto const min_lev = min_element(m_lev_distances.begin(), m_lev_distances.end());

    auto const location = ast::get_source_location(attr);

    if (*min_lev > sema::SIMILAR_LEV)
    {
        errors().emplace_back(
            error::code::SEMA_UNDECLARED_ATTR_IN_ENTRY_GLOBAL,
            location.filename,
            location.line,
            location.column,
            attr.record.lexeme.data());
    }
    else
    {
        errors().emplace_back(
            error::code::SEMA_UNDECLARED_ATTR_IN_ENTRY_GLOBAL_W_HINT,
            location.filename,
            location.line,
            location.column,
            attr.record.lexeme.data(),
            get<ast::decl_attr_node>(*m_flat_attrs[distance(m_lev_distances.begin(), min_lev)])
                .identifier.record.lexeme.data());
    }
}

auto entry_validation_pass::is_attr_declared_for_var(ast::identifier_node const& attr, ast::identifier_node const& var)
    const -> bool
{
    // find the variable declaration
    auto itr = lower_bound(
        m_flat_vars.begin(),
        m_flat_vars.end(),
        to_string_view(var.record.lexeme),
        overloaded{
            [](string_view const& lhs, ast::file_node::children_type* rhs)
            { return lhs < to_string_view(get<ast::decl_var_node>(*rhs).identifier.record.lexeme); },
            [](ast::file_node::children_type* lhs, string_view const& rhs)
            { return to_string_view(get<ast::decl_var_node>(*lhs).identifier.record.lexeme) < rhs; },
        });

    if (itr == m_flat_vars.end()
        || to_string_view(get<ast::decl_var_node>(**itr).identifier.record.lexeme) != to_string_view(var.record.lexeme))
        assert(0); // unreachable

    // find the attribute in the variable declaration
    auto const& var_decl = get<ast::decl_var_node>(**itr);
    return binary_search(
        begin(var_decl.children),
        end(var_decl.children),
        to_string_view(attr.record.lexeme),
        overloaded{ [](string_view const& lhs, ast::file_node::children_type const& rhs)
                    { return lhs < to_string_view(get<ast::identifier_node>(rhs).record.lexeme); },
                    [](ast::file_node::children_type const& lhs, string_view const& rhs)
                    { return to_string_view(get<ast::identifier_node>(lhs).record.lexeme) < rhs; } });
}

auto entry_validation_pass::is_attr_declared_globally(ast::identifier_node const& attr) const -> attr_matcher const*
{
    auto itr = lower_bound(
        m_flat_attrs.begin(),
        m_flat_attrs.end(),
        to_string_view(attr.record.lexeme),
        overloaded{
            [](string_view const& lhs, ast::file_node::children_type* rhs)
            { return lhs < to_string_view(get<ast::decl_attr_node>(*rhs).identifier.record.lexeme); },
            [](ast::file_node::children_type* lhs, string_view const& rhs)
            { return to_string_view(get<ast::decl_attr_node>(*lhs).identifier.record.lexeme) < rhs; },
        });

    if (itr == m_flat_attrs.end()
        || to_string_view(get<ast::decl_attr_node>(**itr).identifier.record.lexeme)
               != to_string_view(attr.record.lexeme))
        return nullptr;

    return &m_matchers[distance(m_flat_attrs.begin(), itr)];
}
