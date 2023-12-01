#pragma once

#include "../lexer/lexed_buffer.h"
#include "../sema/regex_matcher.h"
#include "../sema/typed_header_ast.h"
#include "../sema/typed_log_ast.h"
#include "../common_types.h"
#include "archive_data.h"

#include <string>

auto lex_header(buffer_t const& buffer) -> lexed_buffer;

auto lex_log(buffer_t const& buffer) -> lexed_buffer;

auto parse_header(lexed_buffer const& lexed_buffer) -> typed_header_ast;

auto parse_log(lexed_buffer const& lexed_buffer) -> typed_log_ast;

auto build_typed_header_ast(lexed_buffer const& lexed_buffer, typed_header_ast const& header_ast) -> typed_header_ast;

auto build_typed_log_ast(lexed_buffer const& lexed_buffer, typed_log_ast const& log_ast) -> typed_log_ast;

auto gen_regex_matchers(typed_header_ast const& header_ast) -> std::vector<regex_matcher>;

auto gen_header_data(std::string const& filename, lexed_buffer const& lexed_buffer, typed_header_ast const& header_ast) -> header_data;

auto gen_log_data(std::string const& filename,
                  lexed_buffer const& lexed_buffer,
                  typed_log_ast const& log_ast,
                  header_data const& header_data) -> log_data;
