#pragma once

#include "header_parser_base.h"

#include "../ast/ast.h"
#include "../lexer/lexed_file.h"
#include "../type_aliases.h"

#include <exception>
#include <optional>

struct header_file_ast;
class header_parser_context;

class header_parser : public header_parser_base
{

    lexed_file_walker m_walker;
    ast<ast_node> m_ast;

public:
    header_parser(lexed_file const& file);
    auto gen() -> std::optional<ast<ast_node>>;

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
