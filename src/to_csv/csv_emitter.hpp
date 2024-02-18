//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iosfwd>
#include <string>
#include <vector>

// Writes log lines or vectors of strings to a
// stream in CSV format according to RFC 4180.
class csv_emitter
{
public:
    enum flags
    {
        NONE               = 1u << 0,
        ALIGN              = 1u << 1,
        SORT_COLS_BY_WIDTH = 1u << 2,
    };

    using row_t = std::vector<char const*>;

private:
    flags              m_flags;
    std::vector<int>   m_col_max_width; // used for alignment
    std::vector<row_t> m_rows;

public:
    explicit csv_emitter(flags f);

    void emit(std::ostream& os);
    void add_row(row_t row);

private:
    void emit_unaligned(std::ostream& os);
    void emit_aligned(std::ostream& os);

    void emit_row_unaligned(std::ostream& os, row_t row);
    void emit_row_aligned(std::ostream& os, row_t row);

    void update_col_widths(row_t const& row);
};

inline csv_emitter::flags operator|(csv_emitter::flags a, csv_emitter::flags b)
{
    return static_cast<csv_emitter::flags>(static_cast<int>(a) | static_cast<int>(b));
}

inline csv_emitter::flags& operator|=(csv_emitter::flags& a, csv_emitter::flags b)
{
    return a = a | b;
}

inline csv_emitter::flags operator&(csv_emitter::flags a, csv_emitter::flags b)
{
    return static_cast<csv_emitter::flags>(static_cast<int>(a) & static_cast<int>(b));
}
