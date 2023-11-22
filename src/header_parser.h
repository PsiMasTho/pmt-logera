#pragma once

#include "header_parser_base.h"

#include "header_parser_context.h"
#include "header_lexer.h"

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

struct header_data;

class header_parser : public header_parser_base {
    header_lexer m_lexer;
    header_parser_context& m_ctx;

public:
    explicit header_parser(std::filesystem::path const& path, header_parser_context& ctx);
    std::unique_ptr<header_data> gen();

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

inline int header_parser::lex()
{
    return m_lexer.lex();
}

inline void header_parser::print()
{
}

