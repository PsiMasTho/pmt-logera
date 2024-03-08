//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>

namespace csv
{

template <typename T>
concept row_iterator = requires(T t) {
    {
        *t
    } -> std::convertible_to<std::string_view>;
    {
        ++t
    };
};

enum flags
{
    NONE               = 1u << 0,
    ALIGN              = 1u << 1,
    SORT_COLS_BY_WIDTH = 1u << 2
};

/// \brief Writes log lines or vectors of strings to a
/// stream in CSV format according to RFC 4180.
class emitter
{
    using internal_row = std::vector<std::string_view>;

    std::vector<int>          m_col_max_width; // used for alignment
    std::vector<internal_row> m_rows;
    flags                     m_flags;
    int                       m_lhs_lock;
    bool                      m_sorted; // undone by add_row

public:
    /// \param f flags to control the behavior of the emitter
    /// \param lhs_lock number of columns to not sort when sorting by width
    emitter(flags f, int lhs_lock);

    template <row_iterator T>
    void add_row(T first, T last);

    void emit(std::ostream& os);

private:
    void emit_unaligned(std::ostream& os);
    void emit_aligned(std::ostream& os);

    static void emit_row_unaligned(std::ostream& os, internal_row const& row);
    void        emit_row_aligned(std::ostream& os, internal_row const& row);

    void update_col_widths(internal_row const& row);

    void sort_cols_by_width();
};

} // namespace csv

#include "csv-inl.hpp"
