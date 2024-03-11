//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "logera/errors.hpp"
#include "logera/sema.hpp"
#include "logera/tokens.hpp"

/**
 * @brief Checks that the attribute names in all 'ENTRY_NODE's are unique.
 * The <attr_name, attr_val> pairs are sorted by attr_name.
 * Reports duplicate attribute names.
 */
struct dup_entry_val_check_pass : sema::pass_base<dup_entry_val_check_pass>
{
    using sema::pass_base<dup_entry_val_check_pass>::pass_base;
    void run();
};

namespace error
{

struct duplicate_attr_name_in_entry : with_column, with_formatted_msg
{
    duplicate_attr_name_in_entry(token::source_location location, std::string_view attr_name)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("duplicate attribute name: \'", attr_name, "\' encountered")
    {
    }
};

} // namespace error
