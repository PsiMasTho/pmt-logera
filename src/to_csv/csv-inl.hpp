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