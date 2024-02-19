#pragma once

#include "logera/sema.hpp"

/**
 * @brief Creates 'ENTRY_NODE's from IDENTIFIER_NODE's and
 * 'IDENT_VALUE_PAIR_LIST_NODE's.
 */
struct file_sort_pass : sema::pass_base<file_sort_pass>
{
    using sema::pass_base<file_sort_pass>::pass_base;
    void run(); // 1
};
