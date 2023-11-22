#pragma once

#include <string>
#include <string_view>
#include <memory>

struct ragel_state
{
    int   m_cs = 0;       // current state
    int   m_act = 0;      // current action
    char* m_p = nullptr;  // current pointer
    char* m_pe = nullptr; // pointer end
    char* m_eof = nullptr;// same as m_pe
    char* m_ts = nullptr; // token start
    char* m_te = nullptr; // token end
};

#ifndef MAKE_RAGEL_STATE_AVAILABLE
#define MAKE_RAGEL_STATE_AVAILABLE \
    int& cs = m_state.m_cs;        \
    int& act = m_state.m_act;      \
    char*& p = m_state.m_p;        \
    char*& pe = m_state.m_pe;      \
    char*& eof = m_state.m_eof;    \
    char*& ts = m_state.m_ts;      \
    char*& te = m_state.m_te;
#endif

class lexer_base
{
protected:
    std::string             m_filename;
    std::unique_ptr<char[]> m_buffer;
    int                     m_size;

public:
    explicit lexer_base(char const* infile);
    auto filename() const -> std::string const&;

protected:
    auto line_nr(int ch) const -> int;
};

template <typename derived>
class lexer_crtp : public lexer_base
{
protected:
    ragel_state      m_state;
    int              m_cur_token = 0;
    std::string_view m_cur_matched;

public:
    explicit lexer_crtp(char const* infile);

    auto lex() -> int;
    auto matched() const -> std::string_view;
    auto line_nr() const -> int;

protected:
    void token(auto tok);
};

template <typename derived>
lexer_crtp<derived>::lexer_crtp(char const* infile)
:   lexer_base(infile)
{
    m_state.m_p = m_buffer.get();
    m_state.m_pe = m_buffer.get() + m_size;
    m_state.m_eof = m_state.m_pe;
    static_cast<derived*>(this)->init(m_state);
}

template <typename derived>
auto lexer_crtp<derived>::lex() -> int
{
    static_cast<derived*>(this)->exec(m_state);

    if (m_state.m_cs == -1) // error state
        return 0;

    if (m_state.m_p == m_state.m_pe)
        return 0;
    else
        return m_cur_token;
}

template <typename derived>
auto lexer_crtp<derived>::line_nr() const -> int
{
    auto offset = m_state.m_ts - m_buffer.get();
    return lexer_base::line_nr(offset);
}


template <typename derived>
auto lexer_crtp<derived>::matched() const -> std::string_view
{
    return m_cur_matched;
}

template <typename derived>
void lexer_crtp<derived>::token(auto tok)
{
    m_cur_token = static_cast<int>(tok);
    size_t const size = m_state.m_te - m_state.m_ts;
    m_cur_matched = std::string_view{m_state.m_ts, size};
}