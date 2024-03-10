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