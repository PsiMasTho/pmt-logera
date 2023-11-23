#pragma once

#include "lexed_file.h"

#ifndef MAKE_RAGEL_STATE_AVAILABLE
#define MAKE_RAGEL_STATE_AVAILABLE \
    int& cs = m_cs;        \
    int& act = m_act;      \
    char*& p = m_p;        \
    char*& pe = m_pe;      \
    char*& eof = m_eof;    \
    char*& ts = m_ts;      \
    char*& te = m_te;
#endif

template <typename derived>
class lexer_base
{
protected:
    int   m_cs = 0;       // current state
    int   m_act = 0;      // current action
    char* m_p = nullptr;  // current pointer
    char* m_pe = nullptr; // pointer end
    char* m_eof = nullptr;// same as m_pe
    char* m_ts = nullptr; // token start
    char* m_te = nullptr; // token end

    lexed_file m_result;

public:
    explicit lexer_base(char const* infile);
    auto lex() -> int; // lexes entire file, returns 0 on success, 1 on error
    auto release_result() -> lexed_file; // releases the lexed file

protected:
    void push_token(auto tok); // creates a token from the current state
};

#include "lexer_base-inl.h" // implementation