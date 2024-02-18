#pragma once

#include "logera/sema.hpp"

#include <vector>

/**
 * @brief Sorts the files by name and removes duplicates.
 */
struct date_pass
{
    void operator()(sema::split_trees& trees, std::vector<error::record>& errors) const;
};
