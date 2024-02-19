#pragma once

#include "logera/sema.hpp"

/**
 *  @brief Moves all 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's into two new
 * multifile nodes.
 */
struct split_decl_pass : sema::pass_base<split_decl_pass>
{
    using sema::pass_base<split_decl_pass>::pass_base;
    void run(); // 3
};