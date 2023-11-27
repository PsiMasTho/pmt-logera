#pragma once

#include "header_parser_base.h"

#include "../ast/header_nodes.h"
#include "../lexer/lexed_buffer.h"
#include "../type_aliases.h"

#include <exception>
#include <optional>

struct header_file_ast;
class header_parser_context;

class header_parser : public header_parser_base
{
    lexed_buffer_walker m_walker;
    header_node m_ast;

public:
    header_parser(lexed_buffer const& file);
    auto release_result() -> header_node;

private:
    int parse();
    void error(); // called on (syntax) errors
    int lex(); // returns the next token from the lexical scanner.
    void print(); // use, e.g., d_token, d_loc
    void exceptionHandler(std::exception const& exc);

    // support functions for parse():
    void executeAction_(int ruleNr);
    void errorRecovery_();
    void nextCycle_();
    void nextToken_();
    void print_();
};

inline int header_parser::lex()
{
    m_walker.advance();
    return m_walker.get_cur_token_type();
}

inline void header_parser::print() { }
