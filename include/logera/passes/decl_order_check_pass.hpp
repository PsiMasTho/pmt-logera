//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "logera/ast.hpp"
#include "logera/errors.hpp"
#include "logera/sema.hpp"

/// \brief Checks that the declarations in each file are in alphabetical order.
///        Attr and Var declarations may be mixed as
///        long as they are in alphabetical order with respect to themselves.
struct decl_order_check_pass : sema::pass_base<decl_order_check_pass>
{
    using sema::pass_base<decl_order_check_pass>::pass_base;
    void run();
};

namespace error
{

template <ast::decl_node T>
struct decl_order_violation;

template <>
struct decl_order_violation<ast::decl_var_node> : with_column, with_formatted_msg
{
    decl_order_violation(token::source_location location, std::string_view curr)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("variable declaration: \'", curr, "\' is out of order")
    {
    }
};

template <>
struct decl_order_violation<ast::decl_attr_node> : with_column, with_formatted_msg
{
    decl_order_violation(token::source_location location, std::string_view curr)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("attribute declaration: \'", curr, "\' is out of order")
    {
    }
};

} // namespace error
