#pragma once

#include "opaque_vector.h"
#include "tokens.h"

#include <stdbool.h>

enum ast_node_type
{
                                // tok | owned_str | unowned_str (what) | children (which)
                                //-----+-----------+--------------------+----------------------------------
    MULTIFILE_NODE,             //  N  | N            | N               | Y (FILE_NODE+)
    FILE_NODE,                  //  N  | N            | Y (filename)    | Y (IDENT_VALUE_PAIR_LIST+ or DATE+, DECL_VAR+ or DECL_ATTR+ or IDENTIFIER+ or ENTRY+)
    ENTRY_NODE,                 //  N  | N            | N               | Y (IDENTIFIER, IDENT_VALUE_PAIR_LIST)
    IDENT_VALUE_PAIR_LIST_NODE, //  N  | N            | N               | Y (IDENT_VALUE_PAIR+)
    IDENT_VALUE_PAIR_NODE,      //  N  | N            | N               | Y (IDENTIFIER, ATTR_VALUE)
    DECL_VAR_NODE,              //  N  | N            | N               | Y (IDENTIFIER, IDENTIFIER+)
    DECL_ATTR_NODE,             //  N  | N            | N               | Y (IDENTIFIER, REGEX+)
    DATE_NODE,                  //  Y  | N            | N               | N
    ATTR_VALUE_NODE,            //  Y  | N            | N               | N
    IDENTIFIER_NODE,            //  Y  | N            | N               | N
    REGEX_NODE,                 //  Y  | N            | N               | N
    EMPTY_NODE,                 //  N  | N            | N               | N

    _AST_NODE_TYPE_COUNT
};

typedef struct ast_node_type_properties
{
    char const* name;
    bool has_tok;
    bool has_owned_str;
    bool has_unowned_str;
    bool has_children;
    bool valid_child[_AST_NODE_TYPE_COUNT];
} ast_node_type_properties;

    // encodes the above table
extern ast_node_type_properties const ast_node_type_properties_table[_AST_NODE_TYPE_COUNT];

typedef struct ast_node
{
    enum ast_node_type type;
    union
    {
        char*          owned_str;
        char const*    unowned_str;
        token_record   tok; // owning
    };
    opaque_vector      children; // owning, of ast_node
} ast_node;

void
ast_node_add_child(
    ast_node* self
,   ast_node child
);

ast_node
ast_node_copy(
    ast_node const* self
);

ast_node
ast_node_create(
    int type
);

void
ast_node_destroy(
    void* self
);

source_location  // -> the location of the first child that has a location
ast_node_get_source_location(
    ast_node const* target
);

ast_node
ast_node_move(
    ast_node* self
);
