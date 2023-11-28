#pragma once

#include "../type_aliases.h"
#include "log_date.h"

#include <string>
#include <variant>
#include <vector>

/*
    Note: u32 here represents offsets into the lexed buffer.
*/

struct typed_log_entry
{
    u32 variable;
    std::vector<std::pair<u32, u32>> attr_vals;
};

struct typed_log_date
{
    u32 date;
};

struct typed_log_ast
{
    u32 date;
    std::vector<typed_log_entry> entries;
};
