#include "lexer.h"

#include "algo.h"
#include "errors.h"

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
    [TOKEN_IDENT]      = "[a-zA-Z_][a-zA-Z0-9_\\.]*",
    [TOKEN_REGEX]      = "[^;#\\s]+",
    [TOKEN_ATTR_VALUE] = "[^;#\\s]+",
    [TOKEN_DATE]       = "[0-9]{4}-[0-9]{2}-[0-9]{2}"
};

typedef struct lexer_
{
    char const*       filename;
    char const*       buffer;
    char const*       cursor;
    pcre2_code*       regexes[_TOKEN_COUNT];
    pcre2_match_data* match_datas[_TOKEN_COUNT];
    int               buffer_size;
    int               line_nr;
} lexer_;
// clang-format on

static void lexer_compile_regexes(lexer_handle self);

static void lexer_create_match_datas(lexer_handle self);

static void lexer_destroy_regexes(lexer_handle self);

static void lexer_destroy_match_datas(lexer_handle self);

static void lexer_skip_comment(lexer_handle self);

// ----------------------------------------------------------------------------

lexer_handle lexer_create(void)
{
    lexer_* self = malloc(sizeof(lexer_));
    if (self == NULL)
        FATAL_ERROR("Out of memory");

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
    self->line_nr = 1;
}

void lexer_destroy(lexer_handle self)
{
    assert(self != NULL);

    lexer_destroy_regexes(self);
    lexer_destroy_match_datas(self);
    free(self);
}

// todo: this is extremely slow
source_location lexer_get_location(lexer_handle self, char const* cursor)
{
    assert(self != NULL);

    int column = 0;
    int line = 1;
    for (char const* it = self->buffer; it < cursor; ++it)
        if (*it == '\n')
        {
            ++line;
            column = 0;
        }
        else
            ++column;

    return (source_location){ .filename = self->filename, .line = line, .column = column };
}

char const* lexer_get_cursor(lexer_handle self)
{
    assert(self != NULL);

    return self->cursor;
}

char const* lexer_get_filename(lexer_handle self)
{
    assert(self != NULL);

    return self->filename;
}

int lexer_remaining(lexer_handle self)
{
    assert(self != NULL);

    return distance(self->cursor, self->buffer + self->buffer_size - 1, 1);
}

void lexer_rewind(lexer_handle self, char const* cursor)
{
    assert(self != NULL);
    assert(cursor != NULL);

    while (self->cursor > cursor)
    {
        --self->cursor;
        if (*self->cursor == '\n')
            --self->line_nr;
    }
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

bool lex_token(lexer_handle self, int token, char const** ts, char const** te)
{
    assert(self != NULL);
    assert(token >= 0 && token < _TOKEN_COUNT);

    if (0 > pcre2_match(self->regexes[token], self->cursor, lexer_remaining(self), 0, PCRE2_ANCHORED, self->match_datas[token], NULL))
        return false;

    if (token == TOKEN_NEWLINE)
        ++self->line_nr;

    int const match_len = pcre2_get_ovector_pointer(self->match_datas[token])[1];

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

    char const* ret = strchr(self->cursor, '\n');
    if (ret == NULL)
        self->cursor = self->buffer + self->buffer_size;
    else
        self->cursor = ret;
}
