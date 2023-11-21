//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "csv_emitter.h"
#include "../utility.h"

#include <ostream>
#include <algorithm>
#include <numeric> // iota
#include <utility>

using namespace std;

namespace
{
// A field in an RFC 4180 CSV needs to
// be enclosed in double quotes under certain
// circumstances, for our format, we only
// need to check for a comma.
constexpr auto needs_quotes(string const& str) -> bool
{
    return str.find(',') != string::npos;
}

constexpr auto get_width(string const& str) -> int
{
    int const width = static_cast<int>(str.size());
    return width + (needs_quotes(str) ? 2 : 0);
}

    // one space at the start and 1<= at the end
auto pad(string& str, int width) -> string&
{
    str.insert(0, " ");
    str.append(width - str.size() + 2, ' '); // +2 instead of + 1 to account for the initial space
    return str;
}

auto quote(string& str) -> string&
{
    str.insert(0, "\"");
    str.append("\"");
    return str;
}

auto get_field_unaligned(string& field) -> string&
{
    if (needs_quotes(field))
        quote(field);
    return field;
}

auto get_field_aligned(string& field, int width) -> string
{
    return pad(get_field_unaligned(field), width);
}

void sort_cols_by_width(vector<csv_emitter::row_t>& rows, vector<int>& col_max_width, size_t skip)
{
    auto const width_beg = col_max_width.begin() + skip;

        // sort once to get the indices
    vector<int> indices(distance(width_beg, col_max_width.end()));
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&width_beg](size_t lhs, size_t rhs) { return width_beg[lhs] < width_beg[rhs]; });

        // apply indices to col_max_width
    auto cache = indirect_rearrange(width_beg, col_max_width.end(), indices.begin(), nullptr);

        // apply indices to rows
    for (auto& row : rows)
        cache = indirect_rearrange(row.begin() + skip, row.end(), indices.begin(), move(cache));
}

} // namespace

csv_emitter::csv_emitter(flags f)
    : m_flags{f}
    , m_col_max_width{}
    , m_rows{}
{ }

void csv_emitter::emit(ostream& os)
{
        // only sort the attribute columns (skip first 3)
    if (m_flags & SORT_COLS_BY_WIDTH)
        sort_cols_by_width(m_rows, m_col_max_width, 3);

    if (m_flags & ALIGN)
        emit_aligned(os);
    else
        emit_unaligned(os);
}

void csv_emitter::add_row(row_t row)
{
    update_col_widths(row);
    m_rows.push_back(move(row));
}

void csv_emitter::emit_unaligned(ostream& os)
{
    for (auto& row : m_rows)
        emit_row_unaligned(os, std::move(row));
}

void csv_emitter::emit_aligned(ostream& os)
{
    for (auto& row : m_rows)
        emit_row_aligned(os, std::move(row));
}

void csv_emitter::emit_row_unaligned(ostream& os, row_t row)
{
    string delim;
    for (auto str : row)
            os << exchange(delim, ",") << get_field_unaligned(str);

    os << '\n';
}

void csv_emitter::emit_row_aligned(ostream& os, row_t row)
{
    string delim;
    for (size_t i = 0; i < row.size(); ++i)
        os << exchange(delim, ",") << get_field_aligned(row[i], m_col_max_width[i]);

    os << '\n';
}

void csv_emitter::update_col_widths(row_t const& row)
{
    // resize vec if needed
    m_col_max_width.resize(max(m_col_max_width.size(), row.size()), 0);

    // update widths
    for (size_t i = 0; i < row.size(); ++i)
        m_col_max_width[i] = max(m_col_max_width[i], get_width(row[i]));
}
