#pragma once

#include "tokens.h"

#include <stdbool.h>

extern char const* const token_patterns[_TOKEN_COUNT];

typedef struct lexer_* lexer_handle;

/**
 * @brief Creates a new lexer. Compiles internal regexes.
 * Input buffer must be set before use.
 */
lexer_handle lexer_create(void);

/**
 * @brief Changes the lexer's input buffer.
 * @param buffer Non-owning pointer to the input buffer. Must be null-terminated.
 * @param bufsz Size of the input buffer, in bytes including the null terminator.
 *
 * @note May be called multiple times. Resets the lexer's state.
 */
void lexer_set_buffer(lexer_handle self, char const* filename, char const* buffer, int const bufsz);

void lexer_destroy(lexer_handle self);

source_location lexer_get_location(lexer_handle self, char const* cursor);

char const* lexer_get_cursor(lexer_handle self);

char const* lexer_get_filename(lexer_handle self);

/**
 * @brief Returns the number of bytes remaining in the input buffer, excluding the null terminator.
 */
int lexer_remaining(lexer_handle self);

void lexer_rewind(lexer_handle self, char const* cursor);

/**
 * @brief Skips leading whitespace and comments.
 */
void lexer_skipws(lexer_handle self);

/**
 * @brief Tries to lex the next token. On success, returns true and sets ts and te to the token's
 * [start, end). Also advances the state. On failure, returns false and does not modify ts or te or advance state.
 */
bool lex_token(lexer_handle self, int token, char const** ts, char const** te);

char* strdup_token(char const* ts, char const* te);
