#pragma once

typedef struct ast_node ast_node;
typedef struct opaque_vector opaque_vector;

ast_node // -> FILE_NODE
parse_file(
    char const* filename  // ownership not taken
,   opaque_vector tokens  // vector of token_record, needs to be manually destroyed by the caller, elements may be moved
,   opaque_vector* errors // vector of file_error
);
