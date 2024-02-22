#pragma once

#include "logera/ast.hpp"
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
    void run(); // 8

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