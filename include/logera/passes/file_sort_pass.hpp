//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "logera/errors.hpp"
#include "logera/sema.hpp"

/**
 * @brief Creates 'ENTRY_NODE's from IDENTIFIER_NODE's and
 * 'IDENT_VALUE_PAIR_LIST_NODE's.
 */
struct file_sort_pass : sema::pass_base<file_sort_pass>
{
    using sema::pass_base<file_sort_pass>::pass_base;
    void run();
};

namespace error
{

struct duplicate_filename : with_filename, with_unformatted_msg
{
    duplicate_filename(std::string_view const filename)
        : with_filename(filename)
        , with_unformatted_msg("duplicate filename encountered")
    {
    }
};

} // namespace error