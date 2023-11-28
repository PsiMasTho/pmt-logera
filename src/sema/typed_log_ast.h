#pragma once

#include "../type_aliases.h"
#include "log_date.h"

#include <string>
#include <variant>
#include <vector>

struct typed_log_entry
{
    std::string variable;
    std::vector<std::pair<std::string, std::string>> attr_vals;
};

struct typed_log_date
{
    log_date date;
};

struct typed_log_ast
{
    log_date date;
    std::vector<typed_log_entry> entries;
};
