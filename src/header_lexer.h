#pragma once

#include "lexer_common.h"

class header_lexer : public lexer_crtp<header_lexer>
{
public:
    // inherit ctor
    using lexer_crtp<header_lexer>::lexer_crtp;

    void init(ragel_state& state);
    void exec(ragel_state& state);
};
