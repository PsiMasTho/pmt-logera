#pragma once

#include "logera/sema.hpp"

/**
 * @brief Merges all duplicate 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's into one.
 */
struct merge_dup_decl_pass : sema::pass_base<merge_dup_decl_pass>
{
    using sema::pass_base<merge_dup_decl_pass>::pass_base;
    void run(); // 4
};