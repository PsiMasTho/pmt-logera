#include "tokens.h"

#include "errors.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

char const* const token_names[_TOKEN_COUNT] =
{
    "TOKEN_NEWLINE",
    "TOKEN_COLON",
    "TOKEN_DECL_ATTR",
    "TOKEN_SEMICOLON",
    "TOKEN_DECL_VAR",
    "TOKEN_REGEX",
    "TOKEN_ATTR_VALUE",
    "TOKEN_DATE",
    "TOKEN_IDENT"
};

char const *token_name(int tok)
{
    assert(tok >= 0);
    assert(tok < sizeof(token_names) / sizeof(token_names[0]));
    return token_names[tok];
}

source_location source_location_create(char const* filename, int line, int column)
{
    return (source_location){ .filename = filename, .line = line, .column = column };
}

void token_record_destroy(void* self)
{
    assert(self != NULL);
    token_record* record = (token_record*)self;
    free(record->lexeme);
    record->lexeme = NULL;
}

token_record token_record_move(token_record* self)
{
    assert(self != NULL);
    token_record result = *self;
    self->lexeme = NULL;
    return result;
}

token_record token_record_copy(token_record const* self)
{
    assert(self != NULL);
    token_record result = *self;
    result.lexeme = strdup(self->lexeme);
    return result;
}
