#pragma once

#include "logera/sema.hpp"

/**
 * @brief Checks that all 'DATE_NODE's are unique, valid, once per file,
 * sorted according to filename and first in the file.
 */
struct date_pass : sema::pass_base<date_pass>
{
    using sema::pass_base<date_pass>::pass_base;
    void run(); // 7
};