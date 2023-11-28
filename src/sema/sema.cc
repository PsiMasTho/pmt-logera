#include "sema.h"

void typed_header_ast_builder::sort_statements_by_name()
{
    sort_decl_attr_statements_by_name();
    sort_decl_var_statements_by_name();
}

void typed_header_ast_builder::sort_decl_attr_statements_by_name()
{
    auto const name_cmp = [&](decl_attr_statement const& lhs, decl_attr_statement const& rhs) {
        return m_lex.get_match_at(lhs.attribute) < m_lex.get_match_at(rhs.attribute);
    };

    sort(m_result.decl_attr_statements.begin(), m_result.decl_attr_statements.end(), name_cmp);
}

void typed_header_ast_builder::sort_decl_var_statements_by_name()
{
    auto const name_cmp = [&](decl_var_statement const& lhs, decl_var_statement const& rhs) {
        return m_lex.get_match_at(lhs.variable) < m_lex.get_match_at(rhs.variable);
    };

    sort(m_result.decl_var_statements.begin(), m_result.decl_var_statements.end(), name_cmp);
}

void typed_header_ast_builder::check_statements_for_duplicate_names() { }

void typed_header_ast_builder::sort_statement_values() { }

void typed_header_ast_builder::sort_regexes() { }

void typed_header_ast_builder::sort_attrs() { }

void typed_header_ast_builder::check_statements_for_duplicate_values() { }