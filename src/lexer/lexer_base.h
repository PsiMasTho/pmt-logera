#pragma once

#include "../type_aliases.h"
#include "lexed_buffer.h"

#ifndef MAKE_RAGEL_STATE_AVAILABLE
#    define MAKE_RAGEL_STATE_AVAILABLE                                                                                                     \
        i32& cs = m_cs;                                                                                                                    \
        i32& act = m_act;                                                                                                                  \
        char*& p = m_p;                                                                                                                    \
        char*& pe = m_pe;                                                                                                                  \
        char*& eof = m_eof;                                                                                                                \
        char*& ts = m_ts;                                                                                                                  \
        char*& te = m_te;
#endif

template <typename derived>
class lexer_base
{
protected:
    i32 m_cs = 0; // current state
    i32 m_act = 0; // current action
    char* m_p = nullptr; // current pointer
    char* m_pe = nullptr; // pointer end
    char* m_eof = nullptr; // same as m_pe
    char* m_ts = nullptr; // token start
    char* m_te = nullptr; // token end

    lexed_buffer m_result;

public:
    explicit lexer_base(buffer_t buffer);
    auto lex() -> i32; // lexes entire file, returns 0 on success, 1 on error
    auto release_result() -> lexed_buffer; // releases the lexed file

protected:
    void push_token(auto tok); // creates a token from the current state
};

#include "lexer_base-inl.h" // implementation