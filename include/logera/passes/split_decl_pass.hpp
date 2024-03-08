#pragma once

#include "logera/errors.hpp"
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

namespace error
{

struct no_attributes_declared : with_unformatted_msg
{
    no_attributes_declared()
        : with_unformatted_msg("no attributes declared")
    {
    }
};

struct no_variables_declared : with_unformatted_msg
{
public:
    no_variables_declared()
        : with_unformatted_msg("no variables declared")
    {
    }
};

} // namespace error
