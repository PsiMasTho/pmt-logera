//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "logera/errors.hpp"
#include "logera/sema.hpp"

/**
 * @brief Checks that the values in all 'DECL_VAR_NODE's are the names of
 * 'DECL_ATTR_NODE's. Invalid values are removed and reported.
 */
struct decl_var_val_check_pass : sema::pass_base<decl_var_val_check_pass>
{
    using sema::pass_base<decl_var_val_check_pass>::pass_base;
    void run();
};

namespace error
{
struct undeclared_attr_in_var_decl : with_column, with_formatted_msg
{
    undeclared_attr_in_var_decl(token::source_location location, std::string_view attr_name, std::string_view var_name)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("undeclared attribute: \'", attr_name, "\' in declaration for: \'", var_name, "\'")
    {
    }
};

struct undeclared_attr_in_var_decl_w_hint : with_column, with_formatted_msg
{
    undeclared_attr_in_var_decl_w_hint(
        token::source_location location,
        std::string_view       attr_name,
        std::string_view       var_name,
        std::string_view       hint)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg(
              "undeclared attribute: \'",
              attr_name,
              "\' in declaration for: \'",
              var_name,
              "\'. did you mean: \'",
              hint,
              "\'")
    {
    }
};
} // namespace error
