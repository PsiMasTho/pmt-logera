#pragma once

#include "../type_aliases.h"

enum class header_node_enum : u8
{
    ROOT, // container
    STATEMENT, // container
    DECL_VAR, // container
    DECL_ATTR, // container
    IDENTIFIER, // leaf
    REGEX, // leaf
};