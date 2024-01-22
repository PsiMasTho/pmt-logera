#pragma once

#include "opaque_vector.h"

#include <stdbool.h>

typedef struct ast_node ast_node;

bool check_regex_match(
    ast_node const* decl_attr_root // MULTIFILE_NODE
    ,
    opaque_vector const* matchers,
    ast_node const* attr_node // IDENTIFIER_NODE
    ,
    ast_node const* value_node // ATTR_VALUE_NODE
    ,
    opaque_vector* errors);

opaque_vector // -> vector of vectors of regex_t
create_regex_matchers(ast_node const* decl_attrs, opaque_vector* errors);

bool                          // -> if the given string is a valid date
is_valid_date(char const* str // assumed to be a NTBS in the format "YYYY-MM-DD"
);

// This pass creates 'ENTRY_NODE's from all 'IDENTIFIER_NODE's that are
// followed by 'IDENT_VALUE_PAIR_LIST_NODE's.
// node->type must be MULTIFILE_NODE
// also that all dates are valid and occur once per file
void pass_1(ast_node* node, opaque_vector* errors);

// This pass moves all 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's into two new
// ast's. The returned ast's are a MULTIFILE_NODE, containing one or more
// 'FILE_NODE's. which contain the decl nodes. The decl nodes are sorted by name
// within each file node.
void pass_2(ast_node* node, ast_node* out_decl_attrs, ast_node* out_decl_vars, opaque_vector* errors);

// This pass merges all duplicate 'DECL_ATTR_NODE's and 'DECL_VAR_NODE's into
// one. Then, all duplicate values in the DECL_X_NODE's are removed.
void pass_3(ast_node* in_decl_attrs, ast_node* in_decl_vars, opaque_vector* errors);

// This pass checks that the values in all 'DECL_VAR_NODE's are the names of
// 'DECL_ATTR_NODE's. Invalid values are removed and reported.
void pass_4(ast_node* in_decl_attrs, ast_node* in_decl_vars, opaque_vector* errors);
