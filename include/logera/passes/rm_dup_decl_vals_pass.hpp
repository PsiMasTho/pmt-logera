#pragma once

#include "logera/sema.hpp"

/**
 * @brief Removes all duplicate values in 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's.
 */
struct rm_dup_decl_vals_pass : sema::pass_base<rm_dup_decl_vals_pass>
{
    using sema::pass_base<rm_dup_decl_vals_pass>::pass_base;
    void run(); // 5
};