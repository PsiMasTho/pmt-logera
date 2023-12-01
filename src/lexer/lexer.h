#pragma once

#include "lexer_base.h"

class lexer : public lexer_base<lexer>
{
    friend class lexer_base<lexer>;
    void init();
    void exec();

public:
    using lexer_base<lexer>::lexer_base;

private:

    // splits an ident_value_pair into an ident and a value
    // and pushes them as two tokens
    void push_ident_value_pair(char* start, char* end);
};
