//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace csv
{

enum flags
{
    NONE               = 1u << 0,
    ALIGN              = 1u << 1,
    SORT_COLS_BY_WIDTH = 1u << 2
};

using row = std::vector<char const*>;

// Writes log lines or vectors of strings to a
// stream in CSV format according to RFC 4180.
class emitter
{
    flags            m_flags;
    std::vector<int> m_col_max_width; // used for alignment
    std::vector<row> m_rows;

public:
    explicit emitter(flags f);

    void emit(std::ostream& os);
    void add_row(csv::row row);

private:
    void emit_unaligned(std::ostream& os);
    void emit_aligned(std::ostream& os);

    static void emit_row_unaligned(std::ostream& os, csv::row const& row);
    void        emit_row_aligned(std::ostream& os, csv::row const& row);

    void update_col_widths(csv::row const& row);
};

} // namespace csv
