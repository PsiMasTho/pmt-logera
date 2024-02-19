#pragma once

#include "logera/sema.hpp"

/** @brief Creates 'ENTRY_NODE's from IDENTIFIER_NODE's and
 * 'IDENT_VALUE_PAIR_LIST_NODE's.
 */
struct entry_creation_pass : sema::pass_base<entry_creation_pass>
{
    using sema::pass_base<entry_creation_pass>::pass_base;
    void run(); // 2
};