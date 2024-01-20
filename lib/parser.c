#include "parser.h"
#include "ast.h"
#include "errors.h"
#include "tokens.h"
#include "opaque_vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* EBNF:

<file> ::= <line>*

<line>                  ::= <DATE> <NEWLINE>                  |
                            <declare_attribute> <NEWLINE>     |
                            <declare_variable> <NEWLINE>      |
                            <IDENT> <COLON> <NEWLINE>         |
                            <ident_value_pair_list> <NEWLINE> |
                            <NEWLINE>

<declare_attribute>     ::= <DECL_ATTR> <COLON> <IDENT> <SEMICOLON> (<REGEX> <SEMICOLON>)*

<declare_variable>      ::= <DECL_VAR> <COLON> <IDENT> <SEMICOLON> (<IDENT> <SEMICOLON>)*

<ident_value_pair_list> ::= (<ident_value_pair> <SEMICOLON>)+

<ident_value_pair>      ::= <IDENT> <ATTR_VALUE>

*/

static token_record* peek(opaque_vector const* tokens, int n)
{
    assert(tokens != NULL);
    assert(opaque_vector_size(tokens) >= n);
    
    return opaque_vector_at(tokens, n);
}

static void advance(opaque_vector* tokens, int n)
{
    assert(tokens != NULL);
    assert(opaque_vector_size(tokens) >= n);

    tokens->begin = opaque_vector_at(tokens, n);
}

static ast_node* parse_ident_value_pair(opaque_vector* tokens, ast_node* dest)
{
    assert(tokens != NULL);
    assert(dest != NULL);

    if (opaque_vector_size(tokens) < 2)
        return NULL;

    if (peek(tokens, 0)->token != TOKEN_IDENT || peek(tokens, 1)->token != TOKEN_ATTR_VALUE)
        return NULL;

    *dest = ast_node_create(IDENT_VALUE_PAIR_NODE);
    ast_node ident = ast_node_create(IDENTIFIER_NODE);
    ident.tok = token_record_move(peek(tokens, 0));
    ast_node_add_child(dest, ast_node_move(&ident));
    ast_node attr_value = ast_node_create(ATTR_VALUE_NODE);
    attr_value.tok = token_record_move(peek(tokens, 1));
    ast_node_add_child(dest, ast_node_move(&attr_value));

    advance(tokens, 2);

    return dest;
}

static ast_node* parse_ident_value_pair_list(opaque_vector* tokens, ast_node* dest)
{
    assert(tokens != NULL);
    assert(dest != NULL);

    *dest = ast_node_create(IDENT_VALUE_PAIR_LIST_NODE);

    int n_pairs = 0;
    ast_node ivp;
    while (parse_ident_value_pair(tokens, &ivp))
    {
        ast_node_add_child(dest, ast_node_move(&ivp));
        if (peek(tokens, 0)->token != TOKEN_SEMICOLON)
            break;
        advance(tokens, 1);
        ++n_pairs;
    }

    if (n_pairs == 0)
    {
        ast_node_destroy(dest);
        return NULL;
    }

    return dest;
}

static ast_node* parse_declare_variable(opaque_vector* tokens, ast_node* dest)
{
    assert(tokens != NULL);
    assert(dest != NULL);

    if (opaque_vector_size(tokens) < 4)
        return NULL;

    if (peek(tokens, 0)->token != TOKEN_DECL_VAR || peek(tokens, 1)->token != TOKEN_COLON || peek(tokens, 2)->token != TOKEN_IDENT || peek(tokens, 3)->token != TOKEN_SEMICOLON)
        return NULL;
    
    *dest = ast_node_create(DECL_VAR_NODE);
    ast_node ident = ast_node_create(IDENTIFIER_NODE);
    ident.tok = token_record_move(peek(tokens, 2));
    ast_node_add_child(dest, ast_node_move(&ident));

    advance(tokens, 4);

    while (opaque_vector_size(tokens) >= 2)
    {
        if (peek(tokens, 0)->token != TOKEN_IDENT || peek(tokens, 1)->token != TOKEN_SEMICOLON)
            break;

        ast_node ident = ast_node_create(IDENTIFIER_NODE);
        ident.tok = token_record_move(peek(tokens, 0));
        ast_node_add_child(dest, ast_node_move(&ident));
        advance(tokens, 2);
    }

    return dest;
}

static ast_node* parse_declare_attribute(opaque_vector* tokens, ast_node* dest)
{
    assert(tokens != NULL);
    assert(dest != NULL);

    if (opaque_vector_size(tokens) < 4)
        return NULL;

    if (peek(tokens, 0)->token != TOKEN_DECL_ATTR || peek(tokens, 1)->token != TOKEN_COLON || peek(tokens, 2)->token != TOKEN_IDENT || peek(tokens, 3)->token != TOKEN_SEMICOLON)
        return NULL;
    
    *dest = ast_node_create(DECL_ATTR_NODE);
    ast_node ident = ast_node_create(IDENTIFIER_NODE);
    ident.tok = token_record_move(peek(tokens, 2));
    ast_node_add_child(dest, ast_node_move(&ident));

    advance(tokens, 4);

    while (opaque_vector_size(tokens) >= 2)
    {
        if (peek(tokens, 0)->token != TOKEN_REGEX || peek(tokens, 1)->token != TOKEN_SEMICOLON)
            break;

        ast_node regex = ast_node_create(REGEX_NODE);
        regex.tok = token_record_move(peek(tokens, 0));
        ast_node_add_child(dest, ast_node_move(&regex));
        advance(tokens, 2);
    }

    return dest;
}

static ast_node* parse_line(opaque_vector* tokens, ast_node* dest)
{
    assert(tokens != NULL);
    assert(dest != NULL);

    if (opaque_vector_size(tokens) >= 2 &&
        peek(tokens, 0)->token == TOKEN_DATE &&
        peek(tokens, 1)->token == TOKEN_NEWLINE)
    {
        *dest = ast_node_create(DATE_NODE);
        dest->tok = token_record_move(peek(tokens, 0));
        advance(tokens, 2);
        return dest;
    }

    if (parse_declare_attribute(tokens, dest) &&
        opaque_vector_size(tokens) >= 1 &&
        peek(tokens, 0)->token == TOKEN_NEWLINE)
    {
        advance(tokens, 1);
        return dest;
    }

    if (parse_declare_variable(tokens, dest) &&
        opaque_vector_size(tokens) >= 1 &&
        peek(tokens, 0)->token == TOKEN_NEWLINE)
    {
        advance(tokens, 1);
        return dest;
    }

    if (opaque_vector_size(tokens) >= 3 &&
        peek(tokens, 0)->token == TOKEN_IDENT &&
        peek(tokens, 1)->token == TOKEN_COLON &&
        peek(tokens, 2)->token == TOKEN_NEWLINE)
    {
        *dest = ast_node_create(IDENTIFIER_NODE);
        dest->tok = token_record_move(peek(tokens, 0));
        advance(tokens, 3);
        return dest;
    }

    if (parse_ident_value_pair_list(tokens, dest) &&
        opaque_vector_size(tokens) >= 1 &&
        peek(tokens, 0)->token == TOKEN_NEWLINE)
    {
        advance(tokens, 1);
        return dest;
    }

    if (opaque_vector_size(tokens) >= 1 &&
        peek(tokens, 0)->token == TOKEN_NEWLINE)
    {
        *dest = ast_node_create(EMPTY_NODE);
        advance(tokens, 1);
        return dest;
    }

    return NULL;
}

static char const* error_fmt = "unexpected token \'%s\'";

static void push_parse_error(token_record const* tok, opaque_vector* errors)
{
    assert(tok != NULL);
    assert(errors != NULL);

    file_error error;
    error.message = malloc(1024);
    error.loc = tok->loc;
    snprintf(error.message, 1024, error_fmt, tok->lexeme);
    opaque_vector_push(errors, &error);
}

ast_node parse_file(char const* filename, opaque_vector tokens, opaque_vector* errors)
{
    assert(filename != NULL);

    ast_node root = ast_node_create(FILE_NODE);
    root.unowned_str = filename;

    ast_node line;
    while (parse_line(&tokens, &line))
    {
        if (line.type != EMPTY_NODE)
            ast_node_add_child(&root, line);
    }

    if (opaque_vector_size(&tokens) > 0)
        push_parse_error(peek(&tokens, 0), errors);

    return root;
}
