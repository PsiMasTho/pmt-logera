#pragma once

#include "../type_aliases.h"

#include <string>
#include <variant>
#include <vector>

using typed_header_statement = std::variant<struct decl_attr_statement, struct decl_var_statement>;

struct decl_attr_statement
{
    std::string attribute;
    std::vector<std::string> expressions;
};

struct decl_var_statement
{
    std::string variable;
    std::vector<std::string> attributes;
};

struct typed_log_ast
{
    std::vector<typed_header_statement> statements;
};