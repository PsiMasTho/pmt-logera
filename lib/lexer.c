#include "lexer.h"

#include "algo.h"
#include "errors.h"
#include "opaque_vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

// clang-format off
char const* const token_patterns[_TOKEN_COUNT] =
{
    [TOKEN_COLON]      = ":",
    [TOKEN_SEMICOLON]  = ";",
    [TOKEN_NEWLINE]    = "\\R",
    [TOKEN_KW_ATTR]    = "attr",
    [TOKEN_KW_VAR]     = "var",
    [TOKEN_IDENT]      = "[a-zA-Z_][a-zA-Z0-9_\\.]*", // except keywords
    [TOKEN_REGEX]      = "[^;#\\s]+",
    [TOKEN_ATTR_VALUE] = "[^;#\\s]+",
    [TOKEN_DATE]       = "[0-9]{4}-[0-9]{2}-[0-9]{2}"
};

typedef struct lexer_
{
    opaque_vector     nl_locations; // vector of char const*
    char const*       filename;
    char const*       buffer;
    char const*       cursor;
    pcre2_code*       regexes[_TOKEN_COUNT];
    pcre2_match_data* match_datas[_TOKEN_COUNT];
    int               buffer_size;
} lexer_;
// clang-format on

static int cmp_ptr(void const* lhs, void const* rhs);

static bool token_is_keyword(char const* ts, int len);

static void lexer_find_nl_locations(lexer_handle self);

static void lexer_compile_regexes(lexer_handle self);

static void lexer_create_match_datas(lexer_handle self);

static void lexer_destroy_regexes(lexer_handle self);

static void lexer_destroy_match_datas(lexer_handle self);

static void lexer_skip_comment(lexer_handle self);

static int lexer_get_column_nr_on_line(lexer_handle self, int line_nr, char const* cursor);

// ----------------------------------------------------------------------------

lexer_handle lexer_create(void)
{
    lexer_* self = malloc(sizeof(lexer_));
    if (self == NULL)
        FATAL_ERROR("Out of memory");

    self->nl_locations = opaque_vector_create(sizeof(char const*), opaque_vector_destroy_trivial);
    lexer_compile_regexes(self);
    lexer_create_match_datas(self);

    return self;
}

void lexer_set_buffer(lexer_handle self, char const* filename, char const* buffer, int const bufsz)
{
    assert(self != NULL);
    assert(buffer != NULL);
    assert(bufsz > 0);

    self->filename = filename;
    self->buffer = buffer;
    self->cursor = buffer;
    self->buffer_size = bufsz;

    opaque_vector_clear(&self->nl_locations);
    lexer_find_nl_locations(self);
}

void lexer_destroy(lexer_handle self)
{
    assert(self != NULL);

    opaque_vector_destroy(&self->nl_locations);
    lexer_destroy_regexes(self);
    lexer_destroy_match_datas(self);
    free(self);
}

char const* lexer_get_cursor(lexer_handle self)
{
    assert(self != NULL);

    return self->cursor;
}

source_location lexer_get_location(lexer_handle self, char const* cursor)
{
    assert(self != NULL);
    assert(cursor != NULL);

    int const line_nr = lexer_get_line_nr(self, cursor);

    return (source_location){ .filename = self->filename, .line = line_nr, .column = lexer_get_column_nr_on_line(self, line_nr, cursor) };
}

char const* lexer_get_filename(lexer_handle self)
{
    assert(self != NULL);

    return self->filename;
}

int lexer_get_line_nr(lexer_handle self, char const* cursor)
{
    assert(self != NULL);
    assert(cursor != NULL);

    char const* nl = lower_bound(self->nl_locations.begin, self->nl_locations.end, sizeof(char const*), &cursor, cmp_ptr);

    return distance(self->nl_locations.begin, nl, sizeof(char const*)) + 1;
}

int lexer_get_column_nr(lexer_handle self, char const* cursor)
{
    assert(self != NULL);
    assert(cursor != NULL);

    return lexer_get_column_nr_on_line(self, lexer_get_line_nr(self, cursor), cursor);
}

int lexer_remaining(lexer_handle self)
{
    assert(self != NULL);

    return distance(self->cursor, self->buffer + self->buffer_size, 1);
}

void lexer_set_cursor(lexer_handle self, char const* cursor)
{
    assert(self != NULL);
    assert(cursor != NULL);

    self->cursor = cursor;
}

void lexer_skipws(lexer_handle self)
{
    while (1)
    {
        if (!*self->cursor)
            return;

        if (*self->cursor == '#')
        {
            lexer_skip_comment(self);
            return;
        }

        if (*self->cursor == ' ' || *self->cursor == '\t')
            ++self->cursor;
        else
            return;
    }
}

void lexer_skipline(lexer_handle self)
{
    assert(self != NULL);

    char const* it = strchr(self->cursor, '\n');
    if (it == NULL)
        self->cursor = self->buffer + self->buffer_size;
    else
        self->cursor = it + 1;
}

bool lex_token(lexer_handle self, int token, char const** ts, char const** te)
{
    assert(self != NULL);
    assert(token >= 0 && token < _TOKEN_COUNT);

    if (0 > pcre2_match(self->regexes[token], self->cursor, lexer_remaining(self), 0, PCRE2_ANCHORED, self->match_datas[token], NULL))
        return false;

    int const match_len = pcre2_get_ovector_pointer(self->match_datas[token])[1];

    if (token == TOKEN_IDENT && token_is_keyword(self->cursor, match_len))
        return false;

    if (ts != NULL && te != NULL)
    {
        *ts = self->cursor;
        *te = *ts + match_len;
    }

    self->cursor += match_len;
    return true;
}

char* strdup_token(char const* ts, char const* te)
{
    assert(ts != NULL);
    assert(te != NULL);
    assert(ts <= te);

    int const len = te - ts;
    char* ret = malloc(len + 1);
    if (ret == NULL)
        FATAL_ERROR("Out of memory");

    memcpy(ret, ts, len);
    ret[len] = '\0';
    return ret;
}

// ----------------------------------------------------------------------------

static int cmp_ptr(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    return *(char const**)lhs - *(char const**)rhs;
}

static bool token_is_keyword(char const* ts, int len)
{
    assert(ts != NULL);
    assert(len > 0);

    // either "var" or "attr"
    if (len == 3 && strncmp(ts, "var", 3) == 0)
        return true;
    else if (len == 4 && strncmp(ts, "attr", 4) == 0)
        return true;

    return false;
}

static void lexer_find_nl_locations(lexer_handle self)
{
    assert(self != NULL);

    char const* cursor = self->buffer;
    while (1)
    {
        cursor = strchr(cursor, '\n');
        if (cursor == NULL)
            break;

        opaque_vector_push(&self->nl_locations, &cursor);
        ++cursor;
    }
}

static void lexer_compile_regexes(lexer_handle self)
{
    assert(self != NULL);

    for (int i = 0; i < _TOKEN_COUNT; ++i)
    {
        int errorcode;
        PCRE2_SIZE erroroffset;
        self->regexes[i] = pcre2_compile((PCRE2_SPTR)token_patterns[i], PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroroffset, NULL);
        assert(self->regexes[i] != NULL);
    }
}

static void lexer_create_match_datas(lexer_handle self)
{
    assert(self != NULL);

    for (int i = 0; i < _TOKEN_COUNT; ++i)
        self->match_datas[i] = pcre2_match_data_create_from_pattern(self->regexes[i], NULL);
}

static void lexer_destroy_regexes(lexer_handle self)
{
    assert(self != NULL);

    for (int i = 0; i < _TOKEN_COUNT; ++i)
    {
        pcre2_code_free(self->regexes[i]);
        self->regexes[i] = NULL;
    }
}

static void lexer_destroy_match_datas(lexer_handle self)
{
    assert(self != NULL);

    for (int i = 0; i < _TOKEN_COUNT; ++i)
    {
        pcre2_match_data_free(self->match_datas[i]);
        self->match_datas[i] = NULL;
    }
}

static void lexer_skip_comment(lexer_handle self)
{
    assert(self != NULL);
    assert(*self->cursor == '#');

    char const* it = strchr(self->cursor, '\n');
    if (it == NULL)
        self->cursor = self->buffer + self->buffer_size;
    else
        self->cursor = it;
}

static int lexer_get_column_nr_on_line(lexer_handle self, int line_nr, char const* cursor)
{
    assert(self != NULL);
    assert(line_nr > 0);
    assert(cursor != NULL);

    char const* count_from;
    if (line_nr == 1)
        count_from = self->buffer;
    else
        count_from = *(char const**)opaque_vector_at(&self->nl_locations, line_nr - 2) + 1;

    return distance(count_from, cursor, 1);
}
