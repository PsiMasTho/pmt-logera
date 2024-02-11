#pragma once

#include "meta.hpp"
#include "tokens.hpp"

#include <regex>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

class lexer
{
    std::regex               m_regexes[token::_TYPE_COUNT];
    std::vector<char const*> m_nl_locations;
    char const*              m_filename;
    char const*              m_buffer;
    char const*              m_cursor;
    int                      m_buffer_size;

public:
    enum // for use with lex_token(s)
    {
        SKIPWS = token::_TYPE_COUNT, // effectively calls skipws() from lex_token(s) and always returns true
    };

    lexer();

    void skipws();
    void skipln();

    /**
     * @brief Tries to lex the next token. On success, sets dest to the token and returns true.
     * Also advances the state. On failure, returns false, does not advance state and doesn't modify dest.
     * @param dest Non-owning pointer to the destination string_view. Will be ignored if nullptr.
     */
    template <int tok> auto lex_token(std::string_view* dest) -> bool;

    /**
     * @brief Tries to lex a sequence of tokens.
     * @param dests Tuple of non-owning pointers to the destination string_views. Will be ignored if nullptr.
     */
    template <int... toks> auto lex_tokens(typename meta::repeat_tuple<sizeof...(toks), std::string_view*>::type dests) -> bool;

    /**
     * @brief Changes the lexer's input buffer.
     * @param buffer Non-owning pointer to the input buffer. Must be null-terminated.
     * @param bufsz Size of the input buffer, in bytes including the null terminator.
     *
     * @note May be called multiple times. Resets the lexer's state.
     */
    void set_buffer(char const* filename, char const* buffer, int bufsz);
    void set_cursor(char const* cursor);

    auto get_cursor() const -> char const*;
    auto get_location(char const* cursor) const -> token::source_location;
    auto get_filename() const -> char const*;
    auto get_line_nr(char const* cursor) const -> int;
    auto get_column_nr(char const* cursor) const -> int;

    /**
     * @brief Returns the number of bytes remaining in the input buffer, excluding the null terminator.
     */
    auto get_remaining() const -> std::size_t;

private:
    auto match_literal(std::string_view const literal, std::string_view* dest) -> bool;
    void compile_regexes();
    void skip_comment();
    void find_nl_locations();
    auto get_column_nr_on_line(int line_nr, char const* cursor) const -> int;
};

#include "lexer-inl.hpp"
