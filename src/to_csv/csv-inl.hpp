//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// clang-format off
#ifdef __INTELLISENSE__
    #include "csv.hpp"
#endif
// clang-format on

#include <algorithm>

namespace csv
{

template <row_iterator T>
void emitter::add_row(T first, T last)
{
    internal_row row;
    row.reserve(std::distance(first, last));
    std::copy(first, last, std::back_inserter(row));

    update_col_widths(row);
    m_rows.push_back(std::move(row));
    m_sorted = false;
}

} // namespace csv