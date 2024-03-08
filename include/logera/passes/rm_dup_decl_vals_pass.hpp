#pragma once

#include "logera/errors.hpp"
#include "logera/sema.hpp"
#include "logera/tokens.hpp"

/**
 * @brief Removes all duplicate values in 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's.
 */
struct rm_dup_decl_vals_pass : sema::pass_base<rm_dup_decl_vals_pass>
{
    using sema::pass_base<rm_dup_decl_vals_pass>::pass_base;
    void run(); // 5
};

namespace error
{

struct duplicate_attr_value : with_column, with_formatted_msg
{
    duplicate_attr_value(token::source_location location, std::string_view value)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("duplicate attribute value: \'", value, "\' encountered")
    {
    }
};

struct duplicate_var_value : with_column, with_formatted_msg
{
    duplicate_var_value(token::source_location location, std::string_view value)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("duplicate variable value: \'", value, "\' encountered")
    {
    }
};

} // namespace error