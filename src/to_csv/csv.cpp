//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "csv.hpp"
#include "logera/algo.hpp"

#include <algorithm>
#include <numeric> // iota
#include <ostream>
#include <string_view>
#include <utility>

using namespace std;

namespace
{
// A field in an RFC 4180 CSV needs to
// be enclosed in double quotes under certain
// circumstances, for our format, we only
// need to check for a comma.
auto needs_quotes(string_view const str) -> bool
{
    return str.find(',') != string_view::npos;
}

auto get_width(string_view const str) -> int
{
    int const width = static_cast<int>(str.size());
    return width + (needs_quotes(str) ? 2 : 0);
}

// one space at the start and 1<= at the end
auto pad(string& str, int width) -> string&
{
    str.insert(0, " ");
    str.append(width - str.size() + 2,
               ' '); // +2 instead of + 1 to account for the initial space
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

auto get_field_aligned(string& field, int width) -> string&
{
    return pad(get_field_unaligned(field), width);
}

} // namespace

namespace csv
{

emitter::emitter(flags f, int lhs_lock)
    : m_col_max_width{}
    , m_rows{}
    , m_flags{ f }
    , m_lhs_lock{ lhs_lock }
    , m_sorted{ false }
{
}

void emitter::emit(FILE* output)
{
    if (m_flags & SORT_COLS_BY_WIDTH && !m_sorted)
        sort_cols_by_width();

    if (m_flags & ALIGN)
        emit_aligned(output);
    else
        emit_unaligned(output);
}

void emitter::emit_unaligned(FILE* output)
{
    for (auto& row : m_rows)
        emit_row_unaligned(output, row);
}

void emitter::emit_aligned(FILE* output)
{
    for (auto& row : m_rows)
        emit_row_aligned(output, row);
}

void emitter::emit_row_unaligned(FILE* output, internal_row const& row)
{
    string delim;
    for (auto const str : row)
    {
        fprintf(output, "%s", delim.c_str());
        delim = ",";

        string field(str);
        fprintf(output, "%s", get_field_unaligned(field).c_str());
    }

    fprintf(output, "\n");
}

void emitter::emit_row_aligned(FILE* output, internal_row const& row)
{
    string delim;
    for (size_t i = 0; i < row.size(); ++i)
    {
        fprintf(output, "%s", delim.c_str());
        delim = ",";
        string field(row[i]);
        fprintf(output, "%s", get_field_aligned(field, m_col_max_width[i]).c_str());
    }

    fprintf(output, "\n");
}

void emitter::update_col_widths(internal_row const& row)
{
    // resize vec if needed
    m_col_max_width.resize(max(m_col_max_width.size(), row.size()), 0);

    // update widths
    for (size_t i = 0; i < row.size(); ++i)
        m_col_max_width[i] = max(m_col_max_width[i], get_width(row[i]));
}

void emitter::sort_cols_by_width()
{
    auto const width_beg = m_col_max_width.begin() + m_lhs_lock;

    // sort once to get the indices
    vector<int> indices(distance(width_beg, m_col_max_width.end()));
    iota(indices.begin(), indices.end(), 0);
    sort(
        indices.begin(),
        indices.end(),
        [&width_beg](size_t lhs, size_t rhs) { return width_beg[lhs] < width_beg[rhs]; });

    // apply indices to col_max_width
    auto cache = algo::indirect_rearrange(width_beg, m_col_max_width.end(), indices.begin(), nullptr);

    // apply indices to rows
    for (auto& row : m_rows)
        cache = algo::indirect_rearrange(row.begin() + m_lhs_lock, row.end(), indices.begin(), std::move(cache));

    m_sorted = true;
}

} // namespace csv