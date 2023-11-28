#pragma once

#include "lexer_base.h"

class log_lexer : public lexer_base<log_lexer>
{
    u32 m_n_seq_nl = 0;

    friend class lexer_base<log_lexer>;
    void init();
    void exec();

public:
    using lexer_base<log_lexer>::lexer_base;

private:
    // splits an ident_value_pair into an ident and a value
    // and pushes them as two tokens
    void push_ident_value_pair(char* start, char* end);
};