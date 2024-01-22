#include "../../lib/lexer.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void test_lexer_create(void);

static void test_lexer_lex_token_colons(void);

static void test_lexer_lex_token_idents(void);

static void test_lexer_lex_token_dates(void);

static void test_lexer_rewind(void);

static void test_strdup_token(void);

// ----------------------------------------------------------------------------

void test_lexer(void)
{
    test_lexer_create();
    test_lexer_lex_token_colons();
    test_lexer_lex_token_idents();
    test_lexer_lex_token_dates();
    test_lexer_rewind();
    test_strdup_token();
}

// ----------------------------------------------------------------------------

static void test_lexer_create(void)
{
    lexer_handle lexer = lexer_create();
    assert(lexer != NULL);
    lexer_destroy(lexer);
}

static void test_lexer_lex_token_colons(void)
{
    char const input[] = ": :: ; ;: :; : :";

    lexer_handle lexer = lexer_create();
    lexer_set_buffer(lexer, "test_name", input, sizeof(input));

    char const* ts = NULL;
    char const* te = NULL;

    assert(lex_token(lexer, TOKEN_COLON, &ts, &te));
    lexer_skipws(lexer);

    assert(lex_token(lexer, TOKEN_COLON, &ts, &te));
    assert(lex_token(lexer, TOKEN_COLON, &ts, &te));
    lexer_skipws(lexer);

    assert(!lex_token(lexer, TOKEN_COLON, &ts, &te));
    assert(lex_token(lexer, TOKEN_SEMICOLON, &ts, &te));
    lexer_skipws(lexer);

    assert(lex_token(lexer, TOKEN_SEMICOLON, &ts, &te));
    assert(lex_token(lexer, TOKEN_COLON, &ts, &te));
    lexer_skipws(lexer);

    assert(lex_token(lexer, TOKEN_COLON, &ts, &te));
    assert(lex_token(lexer, TOKEN_SEMICOLON, &ts, &te));
    lexer_skipws(lexer);

    assert(lex_token(lexer, TOKEN_COLON, &ts, &te));
    lexer_skipws(lexer);

    assert(lex_token(lexer, TOKEN_COLON, &ts, &te));

    assert(lexer_remaining(lexer) == 0);

    lexer_destroy(lexer);
}

static void test_lexer_lex_token_idents(void)
{
    char const input[] = "foo    bar baz _jimmy _joe123# comment :) :) #!! \n";
    char const* expected_idents[] = { "foo", "bar", "baz", "_jimmy", "_joe123" };
    lexer_handle lexer = lexer_create();
    lexer_set_buffer(lexer, "test_name", input, sizeof(input));

    char const* ts = NULL;
    char const* te = NULL;

    for (int i = 0; i < sizeof(expected_idents) / sizeof(expected_idents[0]); ++i)
    {
        lexer_skipws(lexer);
        assert(lex_token(lexer, TOKEN_IDENT, &ts, &te));
        assert(strlen(expected_idents[i]) == te - ts);
        assert(memcmp(ts, expected_idents[i], te - ts) == 0);
    }

    assert(!lex_token(lexer, TOKEN_IDENT, &ts, &te));
    assert(!lex_token(lexer, TOKEN_IDENT, &ts, &te));

    char const* expected_nl = "\n";

    lexer_skipws(lexer);
    assert(lex_token(lexer, TOKEN_NEWLINE, &ts, &te));
    assert(strlen(expected_nl) == te - ts);
    assert(memcmp(ts, expected_nl, te - ts) == 0);

    lexer_destroy(lexer);
}

static void test_lexer_lex_token_dates(void)
{
    char const input[] = "2020-01-01 2023-04-05 1998-02-01 foo-01-01";
    char const* expected_dates[] = { "2020-01-01", "2023-04-05", "1998-02-01" };

    lexer_handle lexer = lexer_create();
    lexer_set_buffer(lexer, "test_name", input, sizeof(input));

    char const* ts = NULL;
    char const* te = NULL;

    for (int i = 0; i < sizeof(expected_dates) / sizeof(expected_dates[0]); ++i)
    {
        lexer_skipws(lexer);
        assert(lex_token(lexer, TOKEN_DATE, &ts, &te));
        assert(strlen(expected_dates[i]) == te - ts);
        assert(memcmp(ts, expected_dates[i], te - ts) == 0);
    }

    assert(!lex_token(lexer, TOKEN_DATE, &ts, &te));
    assert(!lex_token(lexer, TOKEN_DATE, &ts, &te));

    lexer_destroy(lexer);
}

static void test_lexer_rewind(void)
{
    char const input[] = "foo\nbar\nbaz";

    lexer_handle lexer = lexer_create();
    lexer_set_buffer(lexer, "test_name", input, sizeof(input));

    char const* ts = NULL;
    char const* te = NULL;

    char const* initial = lexer_get_cursor(lexer);

    assert(lex_token(lexer, TOKEN_IDENT, &ts, &te));
    assert(lex_token(lexer, TOKEN_NEWLINE, &ts, &te));
    assert(lex_token(lexer, TOKEN_IDENT, &ts, &te));

    assert(lexer_get_location(lexer, lexer_get_cursor(lexer)).line == 2);

    lexer_rewind(lexer, initial);

    assert(lexer_get_location(lexer, lexer_get_cursor(lexer)).line == 1);

    lexer_destroy(lexer);
}

static void test_strdup_token(void)
{
    int const n_items = 3;
    char const input[] = "foo    bar    baz";
    char const* expected_results[] = { "foo", "bar", "baz" };
    char* actual_results[n_items];

    lexer_handle lexer = lexer_create();
    lexer_set_buffer(lexer, "test_name", input, sizeof(input));

    char const* ts = NULL;
    char const* te = NULL;

    for (int i = 0; i < n_items; ++i)
    {
        assert(lex_token(lexer, TOKEN_IDENT, &ts, &te));
        actual_results[i] = strdup_token(ts, te);
        lexer_skipws(lexer);
    }

    for (int i = 0; i < n_items; ++i)
    {
        assert(strlen(expected_results[i]) == strlen(actual_results[i]));
        assert(memcmp(expected_results[i], actual_results[i], strlen(expected_results[i])) == 0);
        free(actual_results[i]);
    }

    lexer_destroy(lexer);
}
