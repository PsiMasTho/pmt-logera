#pragma once

#include "parser_base.h"

#include "ast_nodes.h"
#include "../lexer/lexed_buffer.h"
#include "../common_types.h"

#include <exception>

class parser : public parser_base
{
    lexed_buffer_walker m_walker;
    ast_node m_ast;

public:
    parser(lexed_buffer const& file);
    auto release_result() -> ast_node;

private:
    auto parse() -> int;
    void error(); // called on (syntax) errors
    auto lex() -> int; // returns the next token from the lexical scanner.
    void print();
    void exceptionHandler(std::exception const& exc);

    // support functions for parse():
    void executeAction_(int ruleNr);
    void errorRecovery_();
    void nextCycle_();
    void nextToken_();
    void print_();
};

inline auto parser::lex() -> int
{
    m_walker.advance();
    return m_walker.get_cur_token_type();
}

inline void parser::print() { }
