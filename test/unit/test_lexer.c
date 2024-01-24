#include "../../lib/lexer.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void test_lexer_create(void);

static void test_lexer_lex_token_colons(void);

static void test_lexer_lex_token_idents(void);

static void test_lexer_lex_token_dates(void);

static void test_lexer_set_cursor(void);

static void test_lexer_get_line_nr_and_column_nr(void);

static void test_strdup_token(void);

// ----------------------------------------------------------------------------

void test_lexer(void)
{
    test_lexer_create();
    test_lexer_lex_token_colons();
    test_lexer_lex_token_idents();
    test_lexer_lex_token_dates();
    test_lexer_set_cursor();
    test_lexer_get_line_nr_and_column_nr();
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
    lexer_set_buffer(lexer, "test_name", input, sizeof(input) - 1);

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

static void test_lexer_set_cursor(void)
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

    assert(lexer_get_line_nr(lexer, lexer_get_cursor(lexer)) == 2);

    lexer_set_cursor(lexer, initial);

    assert(lexer_get_location(lexer, lexer_get_cursor(lexer)).line == 1);

    assert(lex_token(lexer, TOKEN_IDENT, &ts, &te));
    assert(lex_token(lexer, TOKEN_NEWLINE, &ts, &te));
    assert(lex_token(lexer, TOKEN_IDENT, &ts, &te));
    assert(lex_token(lexer, TOKEN_NEWLINE, &ts, &te));
    assert(lex_token(lexer, TOKEN_IDENT, &ts, &te));

    assert(lexer_get_line_nr(lexer, lexer_get_cursor(lexer)) == 3);

    lexer_destroy(lexer);
}

static void test_lexer_get_line_nr_and_column_nr(void)
{
    char const input[] = "2022-09-15\n"
                         "\n"
                         "\n"
                         "bw:\n"
                         "    weight 71kg;\n"
                         "\n"
                         "dips:\n"
                         "            reps 2s10r ;  rpe 8;\n"
                         "    weight 5kg ; reps 3s8r  ;  rpe 10;\n"
                         "\n"
                         "bench_press:\n"
                         "    weight 60kg  ; reps 4s2r    ; incline 15deg;\n"
                         "    weight 100kg ; reps 5,4,4,1 ; incline 15deg;\n"
                         "\n"
                         "btn_situps:\n"
                         "    weight 2.5kg ; reps 10,10,10,10;\n"
                         "    weight 5kg   ; reps 5s10r;\n"
                         "\n"
                         "treadmill:\n"
                         "    time 10:00;\n";

    lexer_handle lexer = lexer_create();
    lexer_set_buffer(lexer, "test_name", input, sizeof(input));

    char const* p = input;
    int expected_ln = 1;
    int expected_col = 0;
    while (*p)
    {
        assert(lexer_get_line_nr(lexer, p) == expected_ln);
        assert(lexer_get_column_nr(lexer, p) == expected_col);
        if (*p++ == '\n')
        {
            ++expected_ln;
            expected_col = 0;
        }
        else
            ++expected_col;
    }

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
