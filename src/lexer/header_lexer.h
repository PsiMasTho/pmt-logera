#pragma once

#include "lexer_base.h"

class header_lexer : public lexer_base<header_lexer>
{
    friend class lexer_base<header_lexer>;
    void init();
    void exec();

public:
    using lexer_base<header_lexer>::lexer_base;
};
