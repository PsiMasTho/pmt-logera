#pragma once

#include "../type_aliases.h"

#include <string>
#include <variant>
#include <vector>
#include <optional>

using typed_header_statement = std::variant<struct decl_attr_statement, struct decl_var_statement>;

struct decl_attr_statement
{
    u32 attribute;
    std::vector<u32> expressions;
};

struct decl_var_statement
{
    u32 variable;
    std::vector<u32> attributes;
};

struct typed_header_ast
{
    std::vector<decl_attr_statement> decl_attr_statements;
    std::vector<decl_var_statement> decl_var_statements;
};
