#pragma once

#include "../type_aliases.h"
#include "tokens.h"

#include <memory>
#include <string_view>
#include <vector>

class lexed_buffer
{
    std::vector<token_record> m_tokens;
    buffer_t m_buffer;

public:
    lexed_buffer(buffer_t buffer);

    auto get_buffer() -> char*;
    auto get_buffer() const -> char const*;
    auto get_buffer_size() const -> u32;

    auto get_token_count() const -> u32;

    auto get_match_at(token_record tok) const -> std::string_view;
    auto get_match_at(u32 idx) const -> std::string_view;

    auto get_token_record_at(u32 idx) const -> token_record;

    auto get_match_source_location(u32 idx) const -> source_location_t;
    auto get_match_length_at(u32 idx) const -> u16;
    auto get_token_at(u32 idx) const -> token_t;

    auto get_line_nr_at(token_record tok) const -> u32;
    auto get_line_nr_at(u32 idx) const -> u32;

    void push_token_record(token_t tok, u32 offset, u16 len);
};

/*
    Advance must be called once before any of the get_cur_* functions.
*/
class lexed_buffer_walker
{
    lexed_buffer const* m_file;
    u32 m_index;

public:
    lexed_buffer_walker(lexed_buffer const& file);
    auto get_file() const -> lexed_buffer const&;

    void advance();

    auto get_cur_token_record_idx() const -> u32;
    auto get_cur_token_record_source_location() const -> u32;
    auto get_cur_token_type() const -> token_t;
    auto get_cur_match() const -> std::string_view;
    auto get_cur_line_nr() const -> u32;
};