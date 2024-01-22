#pragma once

#include "ast.h"
#include "lexer.h"

ast_node parse_file(lexer_handle lexer, opaque_vector* errors);
