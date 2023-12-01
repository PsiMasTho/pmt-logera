#pragma once

#include "../common_types.h"

#include <optional>
#include <string>
#include <variant>
#include <vector>

using typed_header_statement = std::variant<struct decl_attr_statement, struct decl_var_statement>;

struct decl_attr_statement
{
    tok_rec_idx_t attribute;
    std::vector<tok_rec_idx_t> expressions;
};

struct decl_var_statement
{
    tok_rec_idx_t variable;
    std::vector<tok_rec_idx_t> attributes;
};

struct typed_header_ast
{
    std::vector<decl_attr_statement> decl_attr_statements;
    std::vector<decl_var_statement> decl_var_statements;
};
