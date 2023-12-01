#pragma once

#include "../common_types.h"
#include "log_date.h"

#include <string>
#include <variant>
#include <vector>

struct typed_log_entry
{
    tok_rec_idx_t variable;
    std::vector<std::pair<tok_rec_idx_t, tok_rec_idx_t>> attr_vals;
};

struct typed_log_ast
{
    std::vector<tok_rec_idx> dates; // a valid log should only have one date
    std::vector<typed_log_entry> entries;
};
