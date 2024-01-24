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

static ast_node* parse_line(lexer_handle lexer, ast_node* dest, opaque_vector* errors);

static ast_node* parse_declare_attribute(lexer_handle lexer, ast_node* dest);

static ast_node* parse_declare_variable(lexer_handle lexer, ast_node* dest);

static ast_node* parse_ident_value_pair_list(lexer_handle lexer, ast_node* dest);

static ast_node* parse_ident_value_pair(lexer_handle lexer, ast_node* dest);

static void const* ptr_max(void const* lhs, void const* rhs);

// ----------------------------------------------------------------------------

ast_node parse_file(lexer_handle lexer, opaque_vector* errors)
{
    ast_node root = ast_node_create(FILE_NODE);
    root.unowned_str = lexer_get_filename(lexer);

    ast_node line;
    while (parse_line(lexer, &line, errors))
        if (line.type != EMPTY_NODE)
            ast_node_add_child(&root, line);

    return root;
}

// ----------------------------------------------------------------------------

static ast_node* parse_line(lexer_handle lexer, ast_node* dest, opaque_vector* errors)
{
    assert(dest != NULL);

    lexer_skipws(lexer);
    *dest = ast_node_create(EMPTY_NODE);
    char const *ts, *te, *furthest, *initial = lexer_get_cursor(lexer);

    // <DATE>
    if (lex_token(lexer, TOKEN_DATE, &ts, &te))
    {
        *dest = ast_node_create(DATE_NODE);
        dest->tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_DATE, .location = lexer_get_location(lexer, ts) };
        goto expect_nl; // success
    }
    furthest = lexer_get_cursor(lexer);
    lexer_set_cursor(lexer, initial); // failure, rewind

    // <declare_attribute>
    if (parse_declare_attribute(lexer, dest))
        goto expect_nl; // success
    furthest = ptr_max(furthest, lexer_get_cursor(lexer));
    lexer_set_cursor(lexer, initial); // failure, rewind

    // <declare_variable>
    if (parse_declare_variable(lexer, dest))
        goto expect_nl; // success
    furthest = ptr_max(furthest, lexer_get_cursor(lexer));
    lexer_set_cursor(lexer, initial); // failure, rewind

    // <IDENT> <COLON>
    if (lex_token(lexer, TOKEN_IDENT, &ts, &te))
    {
        lexer_skipws(lexer);
        if (lex_token(lexer, TOKEN_COLON, NULL, NULL))
        {
            *dest = ast_node_create(IDENTIFIER_NODE);
            dest->tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_IDENT, .location = lexer_get_location(lexer, ts) };

            goto expect_nl; // success
        }
    }
    furthest = ptr_max(furthest, lexer_get_cursor(lexer));
    lexer_set_cursor(lexer, initial); // failure, rewind

    // <ident_value_pair_list>
    if (parse_ident_value_pair_list(lexer, dest))
        goto expect_nl; // success
    furthest = ptr_max(furthest, lexer_get_cursor(lexer));
    lexer_set_cursor(lexer, initial); // failure, rewind

expect_nl:
    // <NEWLINE>
    lexer_skipws(lexer);
    if (lex_token(lexer, TOKEN_NEWLINE, NULL, NULL))
        return dest; // success
    furthest = ptr_max(furthest, lexer_get_cursor(lexer));
    lexer_set_cursor(lexer, initial); // failure, rewind

    // end of file
    if (lexer_remaining(lexer) == 1) // 1 because of the null terminator
        return NULL;

    // error
    push_fmted_error(UNEXPECTED_TOKEN, parse_error_fmts, errors, lexer_get_location(lexer, furthest));
    dest->tok.token = EMPTY_NODE;
    lexer_skipline(lexer);
    return dest;
}

static ast_node* parse_declare_attribute(lexer_handle lexer, ast_node* dest)
{
    assert(dest != NULL);
    char const *ts, *te, *initial = lexer_get_cursor(lexer);

    // <KW_ATTR> <COLON> <IDENT> <SEMICOLON>
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_KW_ATTR, NULL, NULL))
        return NULL;
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_COLON, NULL, NULL))
        return NULL;
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_IDENT, &ts, &te))
        return NULL;
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
        return NULL;

    *dest = ast_node_create(DECL_ATTR_NODE);
    ast_node ident = ast_node_create(IDENTIFIER_NODE);
    ident.tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_IDENT, .location = lexer_get_location(lexer, ts) };
    ast_node_add_child(dest, ident);

    // (<REGEX> <SEMICOLON>)*
    while (1)
    {
        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_REGEX, &ts, &te))
            break;
        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
            break;

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
        return NULL;
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_COLON, NULL, NULL))
        return NULL;
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_IDENT, &ts, &te))
        return NULL;
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
        return NULL;

    *dest = ast_node_create(DECL_VAR_NODE);
    ast_node ident = ast_node_create(IDENTIFIER_NODE);
    ident.tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_IDENT, .location = lexer_get_location(lexer, ts) };
    ast_node_add_child(dest, ident);

    // (<IDENT> <SEMICOLON>)*
    while (1)
    {
        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_IDENT, &ts, &te))
            break;
        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
            break;

        ast_node ident = ast_node_create(IDENTIFIER_NODE);
        ident.tok = (token_record){ .lexeme = strdup_token(ts, te), .token = TOKEN_IDENT, .location = lexer_get_location(lexer, ts) };
        ast_node_add_child(dest, ident);
    }

    return dest;
}

static ast_node* parse_ident_value_pair_list(lexer_handle lexer, ast_node* dest)
{
    assert(dest != NULL);

    ast_node tmp = ast_node_create(IDENT_VALUE_PAIR_LIST_NODE);

    int n_pairs = 0;
    ast_node ivp;
    while (1)
    {
        char const* initial = lexer_get_cursor(lexer);
        if (!parse_ident_value_pair(lexer, &ivp))
        {
            lexer_set_cursor(lexer, initial);
            break;
        }

        ast_node_add_child(&tmp, ivp);

        lexer_skipws(lexer);
        if (!lex_token(lexer, TOKEN_SEMICOLON, NULL, NULL))
        {
            lexer_set_cursor(lexer, initial);
            break;
        }

        ++n_pairs;
    }

    // (<ident_value_pair> <SEMICOLON>)+
    if (n_pairs == 0)
    {
        ast_node_destroy(&tmp);
        return NULL;
    }

    *dest = tmp;
    return dest;
}

static ast_node* parse_ident_value_pair(lexer_handle lexer, ast_node* dest)
{
    assert(dest != NULL);

    char const *ts_ident, *te_ident, *ts_val, *te_val;

    // <IDENT> <ATTR_VALUE>
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_IDENT, &ts_ident, &te_ident))
        return NULL;
    lexer_skipws(lexer);
    if (!lex_token(lexer, TOKEN_ATTR_VALUE, &ts_val, &te_val))
        return NULL;

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

static void const* ptr_max(void const* lhs, void const* rhs)
{
    return lhs > rhs ? lhs : rhs;
}
