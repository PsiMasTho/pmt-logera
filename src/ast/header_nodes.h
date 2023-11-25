#pragma once

#include "../type_aliases.h"

enum class header_node_enum : u8
{
    VARIABLE,
    REGEX,
    DECL_VAR,
    NEWLINE,
    END_OF_FILE,
};