#pragma once

#include "lexer_base.h"

class log_lexer : public lexer_base<log_lexer>
{
    friend class lexer_base<log_lexer>;
    void init();
    void exec();

public:
    using lexer_base<log_lexer>::lexer_base;
};