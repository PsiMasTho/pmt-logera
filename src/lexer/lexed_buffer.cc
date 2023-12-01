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

auto lexed_buffer::get_match_at(token_record tok) const -> std::string_view
{
    char const* start = get_buffer() + tok.loc;
    return string_view(start, tok.length);
}

auto lexed_buffer::get_match_at(tok_rec_idx_t idx) const -> string_view
{
    return get_match_at(m_tokens[idx]);
}

auto lexed_buffer::get_token_record_at(tok_rec_idx_t idx) const -> token_record
{
    return m_tokens[idx];
}

auto lexed_buffer::get_match_source_location(tok_rec_idx_t idx) const -> source_location_t
{
    return m_tokens[idx].loc;
}

auto lexed_buffer::get_match_length_at(tok_rec_idx_t idx) const -> u16
{
    return m_tokens[idx].length;
}

auto lexed_buffer::get_token_at(tok_rec_idx_t idx) const -> tok_t
{
    return m_tokens[idx].tok;
}

auto lexed_buffer::get_line_nr_at(token_record tok) const -> u32
{
    return count_line_nr(get_buffer(), tok.loc);
}

auto lexed_buffer::get_line_nr_at(tok_rec_idx_t idx) const -> u32
{
    return get_line_nr_at(m_tokens[idx]);
}

void lexed_buffer::push_token_record(tok_t tok, tok_rec_idx_t idx, u16 len)
{
    m_tokens.emplace_back(idx, len, tok);
}

lexed_buffer_walker::lexed_buffer_walker(lexed_buffer const& file)
    : m_lex(&file)
    , m_index(numeric_limits<u32>::max()) // roll over to 0 on first advance
{ }

auto lexed_buffer_walker::get_file() const -> lexed_buffer const&
{
    return *m_lex;
}

void lexed_buffer_walker::advance()
{
    ++m_index;
}

auto lexed_buffer_walker::get_cur_token_record_idx() const -> tok_rec_idx_t
{
    return m_index;
}

auto lexed_buffer_walker::get_cur_token_record_source_location() const -> source_location_t
{
    return m_lex->get_match_source_location(m_index);
}

auto lexed_buffer_walker::get_cur_token_type() const -> tok_t
{
    return m_lex->get_token_at(m_index);
}

auto lexed_buffer_walker::get_cur_match() const -> string_view
{
    return m_lex->get_match_at(m_index);
}

auto lexed_buffer_walker::get_cur_line_nr() const -> u32
{
    return m_lex->get_line_nr_at(m_index);
}
