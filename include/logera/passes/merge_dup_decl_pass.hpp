#pragma once

#include "logera/errors.hpp"
#include "logera/sema.hpp"
#include "logera/tokens.hpp"

/**
 * @brief Merges all duplicate 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's into one.
 */
struct merge_dup_decl_pass : sema::pass_base<merge_dup_decl_pass>
{
    using sema::pass_base<merge_dup_decl_pass>::pass_base;
    void run();
};

namespace error
{

struct duplicate_attr_decl : with_column, with_formatted_msg
{
    duplicate_attr_decl(token::source_location location, std::string_view attr_name)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("duplicate attribute declaration for: \'", attr_name, "\' encountered")
    {
    }
};

struct duplicate_var_decl : with_column, with_formatted_msg
{
    duplicate_var_decl(token::source_location location, std::string_view var_name)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("duplicate variable declaration for: \'", var_name, "\' encountered")
    {
    }
};

} // namespace error
