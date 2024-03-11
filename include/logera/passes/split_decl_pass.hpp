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

struct mixing_decls_and_nondecls : with_filename, with_unformatted_msg
{
    mixing_decls_and_nondecls(std::string_view const filename)
        : with_filename(filename)
        , with_unformatted_msg("mixing declarations and non-declarations in one file not allowed")
    {
    }
};

} // namespace error
