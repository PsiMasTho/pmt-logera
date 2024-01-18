#pragma once

#include "opaque_vector.h"

opaque_vector // -> vector of token
lex(
    char const* filename,
    char* buffer, 
    int bufsz,
    opaque_vector* errors // vector of file_error
);
