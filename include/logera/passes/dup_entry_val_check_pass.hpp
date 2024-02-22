#pragma once

#include "logera/sema.hpp"

/**
 * @brief Checks that the attribute names in all 'ENTRY_NODE's are unique.
 * The <attr_name, attr_val> pairs are sorted by attr_name.
 * Reports duplicate attribute names.
 */
struct dup_entry_val_check_pass : sema::pass_base<dup_entry_val_check_pass>
{
    using sema::pass_base<dup_entry_val_check_pass>::pass_base;
    void run(); // 9
};