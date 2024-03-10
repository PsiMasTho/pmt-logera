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
    decl_order_violation(token::source_location location, std::string_view prev, std::string_view curr)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg(
              "variable declaration: \'",
              curr,
              "\' is out of order. Previous declaration: \'",
              prev,
              "\'")
    {
    }
};

template <>
struct decl_order_violation<ast::decl_attr_node> : with_column, with_formatted_msg
{
    decl_order_violation(token::source_location location, std::string_view prev, std::string_view curr)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg(
              "attribute declaration: \'",
              curr,
              "\' is out of order. Previous declaration: \'",
              prev,
              "\'")
    {
    }
};

} // namespace error
