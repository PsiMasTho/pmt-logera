#pragma once

#include "lexer_common.h"

class log_lexer : public lexer_crtp<log_lexer>
{
public:
    // inherit ctor
    using lexer_crtp<log_lexer>::lexer_crtp;

    void init(ragel_state& state);
    void exec(ragel_state& state);
};