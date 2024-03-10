#pragma once

#include "logera/ast.hpp"
#include "logera/errors.hpp"
#include "logera/levenshtein.hpp"
#include "logera/sema.hpp"

#include <regex>
#include <vector>

class attr_matcher
{
    std::vector<std::regex> m_regexes;

public:
    /// @note May throw std::regex_error.
    void add_regex(char const* expr);
    auto operator()(char const* str) const -> bool;
};

/// @brief Checks that all 'ENTRY_NODE's have a valid identifier, attrs and attr_values.
class entry_validation_pass : public sema::pass_base<entry_validation_pass>
{
    std::vector<attr_matcher>                   m_matchers;
    std::vector<ast::file_node::children_type*> m_flat_attrs;
    std::vector<ast::file_node::children_type*> m_flat_vars;
    std::vector<int>                            m_lev_distances;
    levenshtein                                 m_lev;

public:
    using sema::pass_base<entry_validation_pass>::pass_base;
    void run();

private:
    void construct_flattened_decls();
    void construct_matchers();
    auto is_entry_variable_declared(ast::entry_node const& entry) const -> bool;
    void report_undeclared_var_in_entry(ast::entry_node const& entry);

    void report_undeclared_attr_for_var(ast::identifier_node const& attr, ast::identifier_node const& var);
    void report_undeclared_attr_globally(ast::identifier_node const& attr);

    void report_regex_mismatch(ast::ident_value_pair_node const& ivp);

    auto is_attr_declared_for_var(ast::identifier_node const& attr, ast::identifier_node const& var) const -> bool;
    auto is_attr_declared_globally(ast::identifier_node const& attr) const -> attr_matcher const*;
};

namespace error
{
struct regcomp_failed : with_column, with_formatted_msg
{
    regcomp_failed(
        token::source_location location,
        std::string_view       regex,
        std::string_view       attr,
        std::string_view       what)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("failed to compile regex: '", regex, "' for attribute: '", attr, "', what: ", what)
    {
    }
};

struct regex_mismatch : with_column, with_formatted_msg
{
    regex_mismatch(token::source_location location, std::string_view attr, std::string_view value)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("regex mismatch for attribute: '", attr, "', value: '", value, "'")
    {
    }
};

struct undeclared_var_in_entry_global : with_column, with_formatted_msg
{
    undeclared_var_in_entry_global(token::source_location location, std::string_view var)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("undeclared variable: '", var, "' in entry")
    {
    }
};

struct undeclared_var_in_entry_global_w_hint : with_column, with_formatted_msg
{
    undeclared_var_in_entry_global_w_hint(token::source_location location, std::string_view var, std::string_view hint)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("undeclared variable: '", var, "' in entry. did you mean: '", hint, "'?")
    {
    }
};

struct undeclared_attr_in_entry_global : with_column, with_formatted_msg
{
    undeclared_attr_in_entry_global(token::source_location location, std::string_view attr)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("undeclared attribute: '", attr, "' in entry")
    {
    }
};

struct undeclared_attr_in_entry_global_w_hint : with_column, with_formatted_msg
{
    undeclared_attr_in_entry_global_w_hint(
        token::source_location location,
        std::string_view       attr,
        std::string_view       hint)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("undeclared attribute: '", attr, "' in entry. did you mean: '", hint, "'?")
    {
    }
};

struct undeclared_attr_in_entry_w_hint : with_column, with_formatted_msg
{
    undeclared_attr_in_entry_w_hint(
        token::source_location location,
        std::string_view       attr,
        std::string_view       var,
        std::string_view       hint)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg(
              "undeclared attribute: '",
              attr,
              "' for variable: '",
              var,
              "' in entry. did you mean: '",
              hint,
              "'?")
    {
    }
};

struct undeclared_attr_in_entry_local_w_hint : with_column, with_formatted_msg
{
    undeclared_attr_in_entry_local_w_hint(
        token::source_location location,
        std::string_view       attr,
        std::string_view       var,
        std::string_view       hint)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg(
              "undeclared attribute: '",
              attr,
              "' for variable: '",
              var,
              "' in entry. did you mean: '",
              hint,
              "'?")
    {
    }
};

struct undeclared_attr_in_entry_local : with_column, with_formatted_msg
{
    undeclared_attr_in_entry_local(token::source_location location, std::string_view attr, std::string_view var)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("undeclared attribute: '", attr, "' for variable: '", var, "' in entry")
    {
    }
};

} // namespace error