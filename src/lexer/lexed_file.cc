#include "lexed_file.h"

#include "../utility/utility.h"

#include <stdexcept>

using namespace std;

lexed_file::lexed_file(char const* filename)
:   m_filename(filename)
,   m_tokens{}
,   m_buffer(nullptr)
,   m_buffer_size(0)
{
    m_buffer = read_file(filename, &m_buffer_size);
    if (!m_buffer)
        throw runtime_error("Failed to read file: " + string(filename));
}

auto lexed_file::get_filename() const -> string const&
{
    return m_filename;
}

auto lexed_file::get_buffer() -> char*
{
    return m_buffer.get();
}

auto lexed_file::get_buffer() const -> char const*
{
    return m_buffer.get();
}

auto lexed_file::get_buffer_size() const -> uint32_t
{
    return m_buffer_size;
}

auto lexed_file::get_token_count() const -> uint32_t
{
    return m_tokens.size();
}

auto lexed_file::get_match_at(uint32_t idx) const -> string_view
{
    char const* start = m_buffer.get() + m_tokens[idx].offset;
    return string_view(start, m_tokens[idx].length);
}

auto lexed_file::get_match_offset_at(uint32_t idx) const -> uint32_t
{
    return m_tokens[idx].offset;
}

auto lexed_file::get_match_length_at(uint32_t idx) const -> uint16_t
{
    return m_tokens[idx].length;
}

auto lexed_file::get_token_at(uint32_t idx) const -> underlying_token_t
{
    return m_tokens[idx].tok;
}

auto lexed_file::get_line_nr_at(uint32_t idx) const -> uint32_t
{
    return count_line_nr(m_buffer.get(), m_tokens[idx].offset);
}

void lexed_file::push_token_record(underlying_token_t tok, uint32_t offset, uint16_t len)
{
    m_tokens.emplace_back(offset, len, tok);
}

lexed_file_walker::lexed_file_walker(lexed_file const& file)
:   m_file(&file)
,   m_index(UINT32_MAX) // roll over on first advance
{ }

auto lexed_file_walker::get_file() const -> lexed_file const&
{
    return *m_file;
}

void lexed_file_walker::advance()
{
    ++m_index;
}

auto lexed_file_walker::get_cur_token_record_offset() const -> uint32_t
{
    return m_index;
}

auto lexed_file_walker::get_cur_token_type() const -> underlying_token_t
{
    return m_file->get_token_at(m_index);
}

auto lexed_file_walker::get_cur_match() const -> string_view
{
    return m_file->get_match_at(m_index);
}

auto lexed_file_walker::get_cur_line_nr() const -> uint32_t
{
    return m_file->get_line_nr_at(m_index);
}

