//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/lexer.hpp"

#include "logera/numeric_cast.hpp"

#include <algorithm>
#include <cassert>

using namespace std;

static char const* const token_patterns[token::_TYPE_COUNT] = { nullptr, // COLON
                                                                nullptr, // SEMICOLON
                                                                "\\r?\\n",
                                                                nullptr,                     // KW_ATTR
                                                                nullptr,                     // KW_VAR
                                                                "[a-zA-Z_][a-zA-Z0-9_\\.]*", // except keywords
                                                                "[^;#\\s]+",
                                                                "[^;#\\s]+",
                                                                "[0-9]{4}-[0-9]{2}-[0-9]{2}" };

lexer::lexer()
    : m_regexes{}
    , m_nl_locations{}
    , m_filename{ nullptr }
    , m_buffer{ nullptr }
    , m_cursor{ nullptr }
    , m_buffer_size{ 0 }
{
    compile_regexes();
}

void lexer::skipws()
{
    assert(m_cursor != nullptr);

    while (1)
    {
        if (*m_cursor == '#')
            return skip_comment();

        if (*m_cursor == ' ' || *m_cursor == '\t')
            ++m_cursor;
        else
            return;
    }
}

void lexer::skipln()
{
    assert(m_cursor != nullptr);

    auto it = find(m_cursor, m_buffer + m_buffer_size, '\n');
    if (it == m_buffer + m_buffer_size)
        m_cursor = it;
    else
        m_cursor = it + 1;
}

void lexer::set_buffer(char const* filename, char const* buffer, size_t bufsz)
{
    assert(filename != nullptr);
    assert(buffer != NULL);
    assert(bufsz > 0);

    m_filename    = filename;
    m_buffer      = buffer;
    m_cursor      = buffer;
    m_buffer_size = bufsz;

    m_nl_locations.clear();
    find_nl_locations();
}

void lexer::set_cursor(char const* cursor)
{
    assert(cursor != nullptr);

    m_cursor = cursor;
}

auto lexer::get_cursor() const -> char const*
{
    assert(m_cursor != nullptr);

    return m_cursor;
}

auto lexer::get_location(char const* cursor) const -> token::source_location
{
    assert(cursor != NULL);

    int const line_nr = get_line_nr(cursor);

    return { .filename = m_filename, .line = line_nr, .column = get_column_nr_on_line(line_nr, cursor) };
}

auto lexer::get_filename() const -> char const*
{
    return m_filename;
}

auto lexer::get_line_nr(char const* cursor) const -> int
{
    assert(cursor != NULL);

    auto const it = lower_bound(m_nl_locations.begin(), m_nl_locations.end(), cursor);
    return numeric_cast<int>(distance(m_nl_locations.begin(), it) + 1);
}

auto lexer::get_column_nr(char const* cursor) const -> int
{
    assert(cursor != NULL);

    return get_column_nr_on_line(get_line_nr(cursor), cursor);
}

auto lexer::get_remaining() const -> size_t
{
    return numeric_cast<size_t>(m_buffer + m_buffer_size - m_cursor);
}

auto lexer::match_literal(string_view const literal, string_view* dest) -> bool
{
    assert(literal.size() > 0);

    if (get_remaining() < literal.size() || !equal(literal.begin(), literal.end(), m_cursor))
        return false;

    if (dest)
        *dest = string_view{ m_cursor, literal.size() };
    m_cursor += literal.size();
    return true;
}

void lexer::compile_regexes()
{
    for (int i = 0; i < token::_TYPE_COUNT; ++i)
        if (token_patterns[i] != nullptr)
            m_regexes[i] = regex(token_patterns[i], regex_constants::optimize);
}

void lexer::skip_comment()
{
    assert(*m_cursor == '#');

    m_cursor = find(m_cursor, m_buffer + m_buffer_size, '\n');
}

void lexer::find_nl_locations()
{
    char const* end    = m_buffer + m_buffer_size;
    char const* cursor = m_buffer;
    while (1)
    {
        cursor = find(cursor, end, '\n');
        if (cursor == end)
            break;

        m_nl_locations.push_back(cursor++);
    }
}

auto lexer::get_column_nr_on_line(int line_nr, char const* cursor) const -> int
{
    assert(line_nr > 0);
    assert(cursor != NULL);

    char const* count_from = [&]
    {
        if (line_nr == 1)
            return m_buffer;
        else
            return m_nl_locations[line_nr - 2] + 1;
    }();

    return numeric_cast<int>(cursor - count_from);
}
