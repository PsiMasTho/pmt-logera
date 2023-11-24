#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "tokens.h" // underlying_token_t

class lexed_file
{
    struct token_record
    {
        u_int32_t          offset; // offset in buffer
        u_int16_t          length; // length of the string
        underlying_token_t tok;   // token (enumeration from tokens.h)
    };

    std::string               m_filename;
    std::vector<token_record> m_tokens;
    std::unique_ptr<char[]>   m_buffer;
    uint32_t                  m_buffer_size;
public:
    lexed_file(char const* filename);
    
    auto get_filename() const -> std::string const&;
    auto get_buffer() -> char*;
    auto get_buffer() const -> char const*;
    auto get_buffer_size() const -> uint32_t;
    auto get_token_count() const -> uint32_t;

    auto get_match_at(uint32_t idx) const -> std::string_view;
    auto get_match_offset_at(uint32_t idx) const -> uint32_t;
    auto get_match_length_at(uint32_t idx) const -> uint16_t;
    auto get_token_at(uint32_t idx) const -> underlying_token_t;
    auto get_line_nr_at(uint32_t idx) const -> uint32_t;

    void push_token_record(underlying_token_t tok, uint32_t offset, uint16_t len);
};

/*
    Advance must be called once before any of the get_cur_* functions.
*/
class lexed_file_walker
{
    lexed_file const* m_file;
    uint32_t m_index;

public:
    lexed_file_walker(lexed_file const& file);
    auto get_file() const -> lexed_file const&;

    void advance();

    auto get_cur_token_record_offset() const -> uint32_t;
    auto get_cur_token_type() const -> underlying_token_t;
    auto get_cur_match() const -> std::string_view;
    auto get_cur_line_nr() const -> uint32_t;
};