#pragma once

#include "../common_types.h"

#include "regex_matcher.h"

#include <vector>
#include <unordered_map>

class typed_header_ast;
class typed_log_ast;

class semantic_checker
{
    std::unordered_map<tok_rec_idx_t, u8> m_errors; // <tok_rec_idx, error_code>

    void sort_statements_by_name(typed_header_ast& ast);
    void sort_decl_attr_statements_by_name(typed_header_ast& ast);
    void sort_decl_var_statements_by_name(typed_header_ast& ast);
    void check_statements_for_duplicate_names(typed_header_ast const& ast);

    void sort_statement_values(typed_header_ast& ast);
    void sort_regexes(typed_header_ast& ast);
    void sort_attrs(typed_header_ast& ast);
    void check_statements_for_duplicate_values(typed_header_ast const& ast);

    void check_var_decl_statements_for_valid_attrs(typed_header_ast const& ast);
};


class log_semantic_checker
{
    void check_all_variables_exist(typed_header_ast const& header, typed_log_ast const& log);
    void check_all_attrs_exist(typed_header_ast const& header, typed_log_ast const& log);
    void check_vars_have_valid_attrs(typed_header_ast const& header, typed_log_ast const& log);
    void check_regexes_match(typed_header_ast const& header, typed_log_ast const& log);
};
