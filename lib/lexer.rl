#include "lexer.h"

#include "errors.h"
#include "io.h"
#include "tokens.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h> // snprintf

#define MATCH_START\
    token_entry = p

#define LINE_START\
    last_nl = p

#define TOKEN_W_LEXEME(tok)\
    push_token_w_lexeme(tok, token_entry, p, source_location_create(filename, current_line, token_entry - last_nl), &records)

#define TOKEN_WO_LEXEME(tok)\
    push_token_wo_lexeme(tok, source_location_create(filename, current_line, token_entry - last_nl), &records)

%%{
    machine lexer;

    alpha_u          = alpha | '_';
    alnum_u          = alnum | '_';
    ws               = space - ['\n''\r'];
    nl               = ('\r'?'\n')                          >{MATCH_START + 1; TOKEN_WO_LEXEME(TOKEN_NEWLINE); ++current_line;} %{LINE_START;};
    kw_var           = ("var")                              >{MATCH_START;}                                                     %{TOKEN_WO_LEXEME(TOKEN_DECL_VAR);};
    kw_attr          = ("attr")                             >{MATCH_START;}                                                     %{TOKEN_WO_LEXEME(TOKEN_DECL_ATTR);};
    colon            = ':'                                  >{MATCH_START;}                                                     %{TOKEN_WO_LEXEME(TOKEN_COLON);};
    semicolon        = ';'                                  >{MATCH_START;}                                                     %{TOKEN_WO_LEXEME(TOKEN_SEMICOLON);};
    kw               = (kw_var | kw_attr);
    ident            = ((alpha_u (alnum_u | '.')*) - kw)    >{MATCH_START;}                                                     %{TOKEN_W_LEXEME(TOKEN_IDENT);};
    regex            = ((print - (space | ';' | '#'))+)     >{MATCH_START;}                                                     %{TOKEN_W_LEXEME(TOKEN_REGEX);};
    date             = (digit{4} '-' digit{2} '-' digit{2}) >{MATCH_START;}                                                     %{TOKEN_W_LEXEME(TOKEN_DATE);};
    attr_value       = ((print - (space | ';' | '#'))+)     >{MATCH_START;}                                                     %{TOKEN_W_LEXEME(TOKEN_ATTR_VALUE);};
    print_non_nl     = print - nl;
    sl_comment       = '#' print_non_nl* nl;
    blank_ln         = ws* (sl_comment | nl);

    date_ln               = ws* date blank_ln;
    decl_attr_ln          = ws* kw_attr colon ws+ ident ws+ semicolon (ws* regex ws* semicolon)* blank_ln;
    decl_var_ln           = ws* kw_var colon ws+ ident ws+ semicolon (ws* ident ws* semicolon)+ blank_ln;
    entry_initial_ln      = ws* ident colon blank_ln;
    entry_attr_val_seq_ln = (ws* ident ws+ attr_value ws* semicolon)+ blank_ln;

    action unrecognised_ln{
        remove_tokens_w_line(current_line, &records);
        push_fmted_error(LEX_ERROR_UNRECOGNIZED_LINE, lex_error_fmts, errors, source_location_create(filename, current_line, 0));
        fgoto skip_ln;
    }

    skip_ln := |*
        (any - nl) => {};
        (nl)          @{fgoto main;};
    *|;

    main := |*
        (date_ln)               => {};
        (decl_attr_ln)          => {};
        (decl_var_ln)           => {};
        (entry_initial_ln)      => {};
        (entry_attr_val_seq_ln) => {};
        (blank_ln)              => {};
        (any)                   => unrecognised_ln;
    *|;
}%%

%% write data;

enum lex_error
{
    LEX_ERROR_UNRECOGNIZED_LINE,
    LEX_ERROR_CANNOT_OPEN_FILE,
    LEX_ERROR_FILE_IO,
    LEX_ERROR_BUFFER_OUT_OF_SPACE,

    _LEX_ERROR_COUNT
};

char const* lex_error_fmts
[_LEX_ERROR_COUNT] =
{
    "unrecognized line",
    "cannot open file '%s'",
    "file io error",
    "buffer out of space"
};

static void
remove_tokens_w_line(
    int const line
,   opaque_vector* const records
){
    assert(records != NULL);

    size_t size = opaque_vector_size(records);
    while (size > 0)
    {
        token_record const* back = opaque_vector_at(records, size - 1);

        if (back->loc.line != line)
            break;

        opaque_vector_pop(records);
        size = opaque_vector_size(records);
    }
}

static void
push_token_w_lexeme(
    int tok
,   char* ts
,   char* te
,   source_location loc
,   opaque_vector* records
){
    int const lexeme_len = te - ts;
    char* lexeme = (char*)malloc(lexeme_len + 1);
    memcpy(lexeme, ts, lexeme_len);
    lexeme[lexeme_len] = '\0';

    token_record rec;
    rec.token = tok;
    rec.loc = loc;
    rec.lexeme = lexeme;

    opaque_vector_push(records, &rec);
}

void
push_token_wo_lexeme(
    int tok
,   source_location loc
,   opaque_vector* records
){
    token_record rec;
    rec.token = tok;
    rec.loc = loc;
    rec.lexeme = NULL;

    opaque_vector_push(records, &rec);
}

opaque_vector
lex(char const* filename
,   char* buffer
,   int bufsz
,   opaque_vector* errors
){
    assert(filename != NULL);
    assert(errors != NULL);
    assert(buffer != NULL);
    assert(bufsz > 1);

            // See ch 5.1 in the ragel manual
    char* p = NULL;   // Data pointer
    char* pe = NULL;  // Data end pointer
    char* eof = NULL; // EOF pointer (== pe for last block, otherwise null)
    char* ts = NULL;  // Token start
    char* te = NULL;  // Token end
    int   cs = 0;     // Current state
    int   act = 0;    // Current action

    char* data = NULL;
    char* token_entry = NULL; // pointer to where a match started
    char* last_nl = buffer;   // pointer to after the last newline
    int leftover = 0;
    int space = 0;
    int len = 0;
    int current_line = 1;

    %% write init;

    opaque_vector records = opaque_vector_create(sizeof(token_record), token_record_destroy);

    FILE* file = fopen(filename, "r");

    if (!file)
    {
        push_fmted_error(LEX_ERROR_CANNOT_OPEN_FILE, lex_error_fmts, errors, source_location_create(NULL, 0, 0), filename);
        return records;
    }

    terminated_reader reader = terminated_reader_create(file, "\n", 1);

    do
    {
        space = bufsz - leftover;

        if (space == 0)
        {
            push_fmted_error(LEX_ERROR_BUFFER_OUT_OF_SPACE, lex_error_fmts, errors, source_location_create(filename, current_line, 0));
            break;
        }

        data = buffer + leftover;
        len = terminated_reader_read(&reader, data, space);

        if (terminated_reader_error(&reader))
        {
            push_fmted_error(LEX_ERROR_FILE_IO, lex_error_fmts, errors, source_location_create(filename, current_line, 0));
            break;
        }

        // find last nl by searching backwards
        p = buffer;
        pe = buffer + leftover + len - 1;
        while (pe > buffer && *pe != '\n')
            pe--;

            // could not find a newline, the buffer is too small
        if (pe++ == buffer)
        {
            push_fmted_error(LEX_ERROR_BUFFER_OUT_OF_SPACE, lex_error_fmts, errors, source_location_create(filename, current_line, 0));
            break;
        }

            // inform ragel if we are at the final buffer block
        eof = terminated_reader_done(&reader) ? pe : NULL;

        %% write exec;

            // How much is still at the end of the buffer?
        leftover = data + len - pe;

        // Move the leftovers to the beginning of the buffer.
        memmove(buffer, pe, leftover);
    }
    while(!terminated_reader_done(&reader));

    return records;
}
