#include "ast.h"

#include "algo.h"
#include "errors.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

ast_node_type_properties const ast_node_type_properties_table[_AST_NODE_TYPE_COUNT] = {
    [MULTIFILE_NODE] = {"MULTIFILE_NODE",              false, false, false, true,  { [FILE_NODE] = true }                                           },
    [FILE_NODE] = { "FILE_NODE",
                        false,                                false,
                        true,                                               true,
                        { [IDENT_VALUE_PAIR_LIST_NODE] = true,
                      [DATE_NODE] = true,
                      [DECL_VAR_NODE] = true,
                      [DECL_ATTR_NODE] = true,
                      [IDENTIFIER_NODE] = true,
                      [ENTRY_NODE] = true }                                                                                                         },
    [ENTRY_NODE] = { "ENTRY_NODE",                 false, false, false, true,  { [IDENTIFIER_NODE] = true, [IDENT_VALUE_PAIR_LIST_NODE] = true }},
    [IDENT_VALUE_PAIR_LIST_NODE] = { "IDENT_VALUE_PAIR_LIST_NODE", false, false, false, true,  { [IDENT_VALUE_PAIR_NODE] = true }                               },
    [IDENT_VALUE_PAIR_NODE]
    = { "IDENT_VALUE_PAIR_NODE",      false, false, false, true,  { [IDENTIFIER_NODE] = true, [ATTR_VALUE_NODE] = true }           },
    [DECL_VAR_NODE] = { "DECL_VAR_NODE",              false, false, false, true,  { [IDENTIFIER_NODE] = true }                                     },
    [DECL_ATTR_NODE] = { "DECL_ATTR_NODE",             false, false, false, true,  { [IDENTIFIER_NODE] = true, [REGEX_NODE] = true }                },
    [DATE_NODE] = { "DATE_NODE",                  true,  false, false, false, {}                                                               },
    [ATTR_VALUE_NODE] = { "ATTR_VALUE_NODE",            true,  false, false, false, {}                                                               },
    [IDENTIFIER_NODE] = { "IDENTIFIER_NODE",            true,  false, false, false, {}                                                               },
    [REGEX_NODE] = { "REGEX_NODE",                 true,  false, false, false, {}                                                               },
    [EMPTY_NODE] = { "EMPTY_NODE",                 false, false, false, false, {}                                                               },
};

void ast_node_add_child(ast_node* const self, ast_node const child)
{
    assert(self != NULL);
    assert(ast_node_type_properties_table[self->type].valid_child[child.type]);
    opaque_vector_push(&self->children, &child);
}

ast_node ast_node_copy(ast_node const* const self)
{
    assert(self != NULL);
    ast_node ret;

    ret.type = self->type;

    if (ast_node_type_properties_table[self->type].has_tok)
        ret.tok = token_record_copy(&self->tok);

    if (ast_node_type_properties_table[self->type].has_owned_str)
        ret.owned_str = strdup(self->owned_str);
    else if (ast_node_type_properties_table[self->type].has_unowned_str)
        ret.unowned_str = self->unowned_str;

    if (ast_node_type_properties_table[self->type].has_children)
    {
        ret.children = opaque_vector_create(sizeof(ast_node), ast_node_destroy);
        opaque_vector_reserve(&ret.children, opaque_vector_size(&self->children));
        for (ast_node* child = self->children.begin; child != self->children.end; ++child)
            ast_node_add_child(&ret, ast_node_copy(child));
    }

    return ret;
}

ast_node ast_node_create(int const type)
{
    ast_node self;
    self.type = type;

    if (ast_node_type_properties_table[type].has_children)
        self.children = opaque_vector_create(sizeof(ast_node), ast_node_destroy);

    return self;
}

void ast_node_destroy(void* const self)
{
    assert(self != NULL);
    ast_node* node = (ast_node*)self;

    if (ast_node_type_properties_table[node->type].has_tok)
        token_record_destroy(&node->tok);

    if (ast_node_type_properties_table[node->type].has_owned_str)
        free(node->owned_str);

    if (ast_node_type_properties_table[node->type].has_children)
        opaque_vector_destroy(&node->children);
}

source_location ast_node_get_source_location(ast_node const* const target)
{
    assert(target != NULL);
    assert(ast_node_type_properties_table[target->type].has_tok || ast_node_type_properties_table[target->type].has_children);

    if (ast_node_type_properties_table[target->type].has_tok)
        return target->tok.location;
    else
    {
        assert(opaque_vector_size(&target->children) > 0);
        return ast_node_get_source_location(target->children.begin);
    }
}

ast_node ast_node_move(ast_node* self)
{
    assert(self != NULL);
    ast_node ret;

    ret.type = self->type;

    if (ast_node_type_properties_table[self->type].has_tok)
        ret.tok = token_record_move(&self->tok);

    if (ast_node_type_properties_table[self->type].has_owned_str)
    {
        ret.owned_str = self->owned_str;
        self->owned_str = NULL;
    }
    else if (ast_node_type_properties_table[self->type].has_unowned_str)
    {
        ret.unowned_str = self->unowned_str;
        self->unowned_str = NULL;
    }

    if (ast_node_type_properties_table[self->type].has_children)
    {
        ret.children = self->children;
        self->children = opaque_vector_create(sizeof(ast_node), ast_node_destroy);
    }

    self->type = EMPTY_NODE;

    return ret;
}
