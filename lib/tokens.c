#include "tokens.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
char const* const token_names[_TOKEN_COUNT] =
{
    [TOKEN_COLON]      = "COLON",
    [TOKEN_SEMICOLON]  = "SEMICOLON",
    [TOKEN_NEWLINE]    = "NEWLINE",
    [TOKEN_KW_ATTR]    = "KW_ATTR",
    [TOKEN_KW_VAR]     = "KW_VAR",
    [TOKEN_IDENT]      = "IDENT",
    [TOKEN_REGEX]      = "REGEX",
    [TOKEN_ATTR_VALUE] = "ATTR_VALUE",
    [TOKEN_DATE]       = "DATE"
};
// clang-format on

// ----------------------------------------------------------------------------

token_record token_record_copy(token_record const* const self)
{
    assert(self != NULL);
    token_record result = *self;
    result.lexeme = strdup(self->lexeme);
    return result;
}

void token_record_destroy(void* const self)
{
    assert(self != NULL);
    token_record* record = (token_record*)self;
    free(record->lexeme);
    record->lexeme = NULL;
}

token_record token_record_move(token_record* const self)
{
    assert(self != NULL);
    token_record result = *self;
    self->lexeme = NULL;
    return result;
}
