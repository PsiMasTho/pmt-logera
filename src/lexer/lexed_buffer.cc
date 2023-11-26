#include "lexed_buffer.h"

#include "../utility/utility.h"

#include <limits>
#include <stdexcept>

using namespace std;

lexed_buffer::lexed_buffer(buffer_t buffer)
    : m_tokens{}
    , m_buffer(std::move(buffer))
{ }

auto lexed_buffer::get_buffer() -> char*
{
    return m_buffer.first.get();
}

auto lexed_buffer::get_buffer() const -> char const*
{
    return m_buffer.first.get();
}

auto lexed_buffer::get_buffer_size() const -> u32
{
    return m_buffer.second;
}

auto lexed_buffer::get_token_count() const -> u32
{
    return m_tokens.size();
}

auto lexed_buffer::get_match_at(u32 idx) const -> string_view
{
    char const* start = get_buffer() + m_tokens[idx].loc;
    return string_view(start, m_tokens[idx].length);
}

auto lexed_buffer::get_match_source_location(u32 idx) const -> u32
{
    return m_tokens[idx].loc;
}

auto lexed_buffer::get_match_length_at(u32 idx) const -> u16
{
    return m_tokens[idx].length;
}

auto lexed_buffer::get_token_at(u32 idx) const -> token_t
{
    return m_tokens[idx].tok;
}

auto lexed_buffer::get_line_nr_at(u32 idx) const -> u32
{
    return count_line_nr(get_buffer(), m_tokens[idx].loc);
}

void lexed_buffer::push_token_record(token_t tok, u32 loc, u16 len)
{
    m_tokens.emplace_back(loc, len, tok);
}

lexed_buffer_walker::lexed_buffer_walker(lexed_buffer const& file)
    : m_file(&file)
    , m_index(numeric_limits<u32>::max()) // roll over to 0 on first advance
{ }

auto lexed_buffer_walker::get_file() const -> lexed_buffer const&
{
    return *m_file;
}

void lexed_buffer_walker::advance()
{
    ++m_index;
}

auto lexed_buffer_walker::get_cur_token_record_source_location() const -> u32
{
    return m_index;
}

auto lexed_buffer_walker::get_cur_token_type() const -> token_t
{
    return m_file->get_token_at(m_index);
}

auto lexed_buffer_walker::get_cur_match() const -> string_view
{
    return m_file->get_match_at(m_index);
}

auto lexed_buffer_walker::get_cur_line_nr() const -> u32
{
    return m_file->get_line_nr_at(m_index);
}
