//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "logera/sema.hpp"

/** @brief Creates 'ENTRY_NODE's from IDENTIFIER_NODE's and
 * 'IDENT_VALUE_PAIR_LIST_NODE's.
 */
struct entry_creation_pass : sema::pass_base<entry_creation_pass>
{
    using sema::pass_base<entry_creation_pass>::pass_base;
    void run();
};

namespace error
{
struct ident_without_value_list : with_column, with_unformatted_msg
{
    ident_without_value_list(token::source_location location)
        : with_column(location.filename, location.line, location.column)
        , with_unformatted_msg("identifier without value list encountered")
    {
    }
};

struct value_list_without_ident : with_column, with_unformatted_msg
{
    value_list_without_ident(token::source_location location)
        : with_column(location.filename, location.line, location.column)
        , with_unformatted_msg("value list without identifier encountered")
    {
    }
};
} // namespace error