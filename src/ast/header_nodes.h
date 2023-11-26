#pragma once

#include "../type_aliases.h"

enum class header_node_enum : u8
{
    ROOT,       // container
    STATEMENT,  // container
    //VARIABLE,   // container
    IDENTIFIER, // leaf
    REGEX,      // leaf
    DECL_VAR,   // container
    DECL_ATTR,  // container
};