#include "logera/lexer.hpp"

#include <cassert>
#include <string>

using namespace std;

static void test_lexer_lex_token_colons();

static void test_lexer_lex_token_idents();

static void test_lexer_lex_token_dates();

static void test_lexer_set_cursor();

static void test_lexer_get_line_nr_and_column_nr();

static void test_strdup_token();

void test_lexer()
{
    test_lexer_lex_token_colons();
    test_lexer_lex_token_idents();
    test_lexer_lex_token_dates();
    test_lexer_set_cursor();
    test_lexer_get_line_nr_and_column_nr();
    test_strdup_token();
}

static void test_lexer_lex_token_colons()
{
    string input = ": :: ; ;: :; : :";

    lexer l;
    l.set_buffer("test_name", input.c_str(), input.size());

    string_view tok;

    assert(l.lex_token<token::COLON>(&tok));
    l.skipws();

    assert(l.lex_token<token::COLON>(&tok));
    assert(l.lex_token<token::COLON>(&tok));
    l.skipws();

    assert(!l.lex_token<token::COLON>(&tok));
    assert(l.lex_token<token::SEMICOLON>(&tok));
    l.skipws();

    assert(l.lex_token<token::SEMICOLON>(&tok));
    assert(l.lex_token<token::COLON>(&tok));
    l.skipws();

    assert(l.lex_token<token::COLON>(&tok));
    assert(l.lex_token<token::SEMICOLON>(&tok));
    l.skipws();

    assert(l.lex_token<token::COLON>(&tok));
    l.skipws();

    assert(l.lex_token<token::COLON>(&tok));

    assert(l.get_remaining() == 0);
}

static void test_lexer_lex_token_idents()
{
    string      input             = "foo    bar baz _jimmy _joe123# comment :) :) #!! \n";
    string_view expected_idents[] = { "foo", "bar", "baz", "_jimmy", "_joe123" };
    lexer       l;
    l.set_buffer("test_name", input.c_str(), input.size());

    string_view tok;

    for (size_t i = 0; i < sizeof(expected_idents) / sizeof(expected_idents[0]); ++i)
    {
        l.skipws();
        assert(l.lex_token<token::IDENT>(&tok));
        assert(expected_idents[i] == tok);
    }

    assert(!l.lex_token<token::IDENT>(&tok));
    assert(!l.lex_token<token::IDENT>(&tok));

    l.skipws();
    assert(l.lex_token<token::NEWLINE>(&tok));
    assert(tok == "\n");
}

static void test_lexer_lex_token_dates()
{
    string      input            = "2020-01-01 2023-04-05 1998-02-01 foo-01-01";
    string_view expected_dates[] = { "2020-01-01", "2023-04-05", "1998-02-01" };

    lexer l;
    l.set_buffer("test_name", input.c_str(), input.size());

    string_view tok;

    for (size_t i = 0; i < sizeof(expected_dates) / sizeof(expected_dates[0]); ++i)
    {
        l.skipws();
        assert(l.lex_token<token::DATE>(&tok));
        assert(expected_dates[i] == tok);
    }

    assert(!l.lex_token<token::DATE>(&tok));
    assert(!l.lex_token<token::DATE>(&tok));
}

static void test_lexer_set_cursor()
{
    string input = "foo\nbar\nbaz";

    lexer l;
    l.set_buffer("test_name", input.c_str(), input.size());

    string_view tok;

    char const* initial = l.get_cursor();

    assert(l.lex_token<token::IDENT>(&tok));
    assert(l.lex_token<token::NEWLINE>(&tok));
    assert(l.lex_token<token::IDENT>(&tok));

    assert(l.get_line_nr(l.get_cursor()) == 2);

    l.set_cursor(initial);

    assert(l.get_location(l.get_cursor()).line == 1);

    assert(l.lex_token<token::IDENT>(&tok));
    assert(l.lex_token<token::NEWLINE>(&tok));
    assert(l.lex_token<token::IDENT>(&tok));
    assert(l.lex_token<token::NEWLINE>(&tok));
    assert(l.lex_token<token::IDENT>(&tok));

    assert(l.get_line_nr(l.get_cursor()) == 3);
}

static void test_lexer_get_line_nr_and_column_nr()
{
    string input = "2022-09-15\n"
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

    lexer l;
    l.set_buffer("test_name", input.c_str(), input.size());

    char const* p            = input.c_str();
    int         expected_ln  = 1;
    int         expected_col = 0;
    while (*p)
    {
        assert(l.get_line_nr(p) == expected_ln);
        assert(l.get_column_nr(p) == expected_col);
        if (*p++ == '\n')
        {
            ++expected_ln;
            expected_col = 0;
        }
        else
            ++expected_col;
    }
}

static void test_strdup_token()
{
    int const   n_items            = 3;
    string      input              = "foo    bar    baz";
    char const* expected_results[] = { "foo", "bar", "baz" };
    string_view actual_results[n_items];

    lexer l;
    l.set_buffer("test_name", input.c_str(), input.size());

    string_view tok;

    for (int i = 0; i < n_items; ++i)
    {
        assert(l.lex_token<token::IDENT>(&tok));
        actual_results[i] = tok;
        l.skipws();
    }

    for (int i = 0; i < n_items; ++i)
        assert(actual_results[i] == expected_results[i]);
}
