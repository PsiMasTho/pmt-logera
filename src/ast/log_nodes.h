#pragma once

#include "../type_aliases.h"

enum class log_node_enum : u8
{
    ROOT, // container
    STATEMENT, // container
    DATE, // leaf
    VARIABLE, // container
    IDENTIFIER, // leaf
    IDENT_VALUE_PAIR, // leaf (todo: container?)
    IDENT_VALUE_PAIR_LIST // container
};
