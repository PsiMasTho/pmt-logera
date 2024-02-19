#pragma once

#include "logera/sema.hpp"

/**
 * @brief Checks that the values in all 'DECL_VAR_NODE's are the names of
 * 'DECL_ATTR_NODE's. Invalid values are removed and reported.
 */
struct decl_var_val_check_pass : sema::pass_base<decl_var_val_check_pass>
{
    using sema::pass_base<decl_var_val_check_pass>::pass_base;
    void run(); // 6
};