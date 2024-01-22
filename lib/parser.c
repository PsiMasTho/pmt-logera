#include "parser.h"

#include "ast.h"
#include "errors.h"
#include "lexer.h"
#include "opaque_vector.h"
#include "tokens.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
/* EBNF:

<file> ::= <line>*

<line>                  ::= <DATE> <NEWLINE>                  |
                            <declare_attribute> <NEWLINE>     |
                            <declare_variable> <NEWLINE>      |
                            <IDENT> <COLON> <NEWLINE>         |
                            <ident_value_pair_list> <NEWLINE> |
                            <NEWLINE>

<declare_attribute>     ::= <KW_ATTR> <COLON> <IDENT> <SEMICOLON> (<REGEX> <SEMICOLON>)*

<declare_variable>      ::= <KW_VAR> <COLON> <IDENT> <SEMICOLON> (<IDENT> <SEMICOLON>)*

<ident_value_pair_list> ::= (<ident_value_pair> <SEMICOLON>)+

<ident_value_pair>      ::= <IDENT> <ATTR_VALUE>

*/
// clang-format on

enum parse_error
{
    UNEXPECTED_TOKEN,

    _PARSE_ERROR_COUNT
};

static char const* parse_error_fmts[_PARSE_ERROR_COUNT] = { [UNEXPECTED_TOKEN] = "unexpected token" };

static ast_node* parse_line(lexer_handle lexer, ast_node* dest);

static ast_node* parse_declare_attribute(lexer_handle lexer, ast_node* dest);

static ast_node* parse_declare_variable(lexer_handle lexer, ast_node* dest);

static ast_node* parse_ident_value_pair_list(lexer_handle lexer, ast_node* dest);

static ast_node* parse_ident_value_pair(lexer_handle lexer, ast_node* dest);

// ----------------------------------------------------------------------------

ast_node parse_file(lexer_handle lexer, opaque_vector* errors)
{
    ast_node root = ast_node_create(FILE_NODE);
    root.unowned_str = lexer_get_filename(lexer);

    ast_node line;
    while (parse_line(lexer, &line))
        if (line.type != EMPTY_NODE)
            ast_node_add_child(&root, line);

    if (lexer_remaining(lexer) > 0)
        push_fmted_error(UNEXPECTED_TOKEN, parse_error_fmts, errors, lexer_get_location(lexer, lexer_get_cursor(lexer)));

    return root;
}

// ----------------------------------------------------------------------------

static ast_node* parse_line(lexer_handle lexer, ast_node* dest)
{
    assert(dest != NULL);

    lexer_skipws(lexer);
    char const *ts, *te, *initial = lexer_get_cursor(lexer);

    // <DATE> <NEWLINE>
    if (lex_token(lexer, TOKEN_DATE, &ts, &te))
    {
        lexer_skipws(lexer);
        if (lex_token(lexer, TOKEN_NEWLINE, NULL, NULL))
        {
            *dest = ast_node_create(DATE_NODE);
            dest->tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_DATE, .location = lexer_get_location(lexer, ts) };
            return dest; // success
        }
    }
    lexer_rewind(lexer, initial); // failure, rewind

    // <declare_attribute> <NEWLINE>
    if (parse_declare_attribute(lexer, dest))
    {
        lexer_skipws(lexer);
        if (lex_token(lexer, TOKEN_NEWLINE, NULL, NULL))
            return dest; // success
    }
    lexer_rewind(lexer, initial); // failure, rewind

    // <declare_variable> <NEWLINE>
    if (parse_declare_variable(lexer, dest))
    {
        lexer_skipws(lexer);
        if (lex_token(lexer, TOKEN_NEWLINE, NULL, NULL))
            return dest; // success
    }
    lexer_rewind(lexer, initial); // failure, rewind

    // <IDENT> <COLON> <NEWLINE>
    if (lex_token(lexer, TOKEN_IDENT, &ts, &te))
    {
        lexer_skipws(lexer);
        if (lex_token(lexer, TOKEN_COLON, NULL, NULL))
        {
            lexer_skipws(lexer);
            if (lex_token(lexer, TOKEN_NEWLINE, NULL, NULL))
            {
                *dest = ast_node_create(IDENTIFIER_NODE);
                dest->tok
                    = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_IDENT, .location = lexer_get_location(lexer, ts) };
                return dest; // success
            }
        }
    }
    lexer_rewind(lexer, initial); // failure, rewind

    // <ident_value_pair_list> <NEWLINE>
    if (parse_ident_value_pair_list(lexer, dest))
    {
        lexer_skipws(lexer);
        if (lex_token(lexer, TOKEN_NEWLINE, NULL, NULL))
            return dest; // success
    }
    lexer_rewind(lexer, initial); // failure, rewind

    // <NEWLINE>
    if (lex_token(lexer, TOKEN_NEWLINE, NULL, NULL))
    {
        *dest = ast_node_create(EMPTY_NODE);
        return dest; // success
    }
    lexer_rewind(lexer, initial); // failure, rewind

    return NULL;
}

static ast_node* parse_declare_attribute(lexer_handle lexer, ast_node* dest)
{
    assert(dest != NULL);
    char const *ts, *te, *initial = lexer_get_cursor(lexer);

    // <KW_ATTR> <COLON> <IDENT> <SEMICOLON>
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_KW_ATTR, NULL, NULL))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_COLON, NULL, NULL))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_IDENT, &ts, &te))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }

    *dest = ast_node_create(DECL_ATTR_NODE);
    ast_node ident = ast_node_create(IDENTIFIER_NODE);
    ident.tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_IDENT, .location = lexer_get_location(lexer, ts) };
    ast_node_add_child(dest, ident);

    // (<REGEX> <SEMICOLON>)*
    while (1)
    {
        initial = lexer_get_cursor(lexer);

        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_REGEX, &ts, &te))
        {
            lexer_rewind(lexer, initial);
            break;
        }
        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
        {
            lexer_rewind(lexer, initial);
            break;
        }

        ast_node regex = ast_node_create(REGEX_NODE);
        regex.tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_REGEX, .location = lexer_get_location(lexer, ts) };
        ast_node_add_child(dest, regex);
    }

    return dest;
}

static ast_node* parse_declare_variable(lexer_handle lexer, ast_node* dest)
{
    assert(dest != NULL);
    char const *ts, *te, *initial = lexer_get_cursor(lexer);

    // <KW_VAR> <COLON> <IDENT> <SEMICOLON>
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_KW_VAR, NULL, NULL))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_COLON, NULL, NULL))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_IDENT, &ts, &te))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }

    *dest = ast_node_create(DECL_VAR_NODE);
    ast_node ident = ast_node_create(IDENTIFIER_NODE);
    ident.tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_IDENT, .location = lexer_get_location(lexer, ts) };
    ast_node_add_child(dest, ident);

    // (<IDENT> <SEMICOLON>)*
    while (1)
    {
        initial = lexer_get_cursor(lexer);

        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_IDENT, &ts, &te))
        {
            lexer_rewind(lexer, initial);
            break;
        }
        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
        {
            lexer_rewind(lexer, initial);
            break;
        }

        ast_node ident = ast_node_create(IDENTIFIER_NODE);
        ident.tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_IDENT, .location = lexer_get_location(lexer, ts) };
        ast_node_add_child(dest, ident);
    }

    return dest;
}

static ast_node* parse_ident_value_pair_list(lexer_handle lexer, ast_node* dest)
{
    assert(dest != NULL);

    *dest = ast_node_create(IDENT_VALUE_PAIR_LIST_NODE);

    char const* initial;
    int n_pairs = 0;
    ast_node ivp;
    while (1)
    {
        char const* initial = lexer_get_cursor(lexer);

        if (!parse_ident_value_pair(lexer, &ivp))
        {
            lexer_rewind(lexer, initial);
            break;
        }
        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
        {
            lexer_rewind(lexer, initial);
            break;
        }
        ast_node_add_child(dest, ivp);
        ++n_pairs;
    }

    // (<ident_value_pair> <SEMICOLON>)+
    if (n_pairs == 0)
    {
        ast_node_destroy(dest);
        return NULL;
    }

    return dest;
}

static ast_node* parse_ident_value_pair(lexer_handle lexer, ast_node* dest)
{
    assert(dest != NULL);

    char const *ts_ident, *te_ident, *ts_val, *te_val, *initial = lexer_get_cursor(lexer);

    // <IDENT> <ATTR_VALUE>
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_IDENT, &ts_ident, &te_ident))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_ATTR_VALUE, &ts_val, &te_val))
    {
        lexer_rewind(lexer, initial);
        return NULL;
    }

    *dest = ast_node_create(IDENT_VALUE_PAIR_NODE);
    ast_node ident = ast_node_create(IDENTIFIER_NODE);
    ident.tok = (token_record){ .lexeme = strdup_token(ts_ident, te_ident),
                                .token = TOKEN_IDENT,
                                .location = lexer_get_location(lexer, ts_ident) };
    ast_node_add_child(dest, ident);
    ast_node attr_value = ast_node_create(ATTR_VALUE_NODE);
    attr_value.tok = (token_record){ .lexeme = strdup_token(ts_val, te_val),
                                     .token = TOKEN_ATTR_VALUE,
                                     .location = lexer_get_location(lexer, ts_val) };
    ast_node_add_child(dest, attr_value);

    return dest;
}
