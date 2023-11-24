#pragma once

#include "log_parser_base.h"

#include "log_parser_context.h"
#include "../lexer/lexed_file.h"
#include "../ast/log_file_ast.h"

#include <filesystem>
#include <memory>
#include <string>
#include <optional>

struct log_file_ast;
class log_parser_context;

class log_parser : public log_parser_base {

    lexed_file_walker m_walker;
    log_file_ast m_ast;

public:
    log_parser(lexed_file const& file, offset_t lexed_file_offset);
    auto gen() -> std::optional<log_file_ast>;

private:
    int  parse();
    void error(); // called on (syntax) errors
    int  lex(); // returns the next token from the lexical scanner.
    void print(); // use, e.g., d_token, d_loc
    void exceptionHandler(std::exception const& exc);

    // support functions for parse():
    void executeAction_(int ruleNr);
    void errorRecovery_();
    void nextCycle_();
    void nextToken_();
    void print_();
};

#include <iostream>

inline int log_parser::lex()
{
    m_walker.advance();
    return m_walker.get_cur_token_type();
}

inline void log_parser::print()
{
}
