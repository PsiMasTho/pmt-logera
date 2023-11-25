#pragma once

#include "../type_aliases.h"

enum class log_node_enum : u8
{
    ROOT,
    STATEMENT,
    DATE,
    VARIABLE,
    NEWLINE,
    END_OF_FILE,
    IDENT_VALUE_PAIR,
    IDENT_VALUE_PAIR_LIST
};
