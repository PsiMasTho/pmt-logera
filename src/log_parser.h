#pragma once

#include "log_parser_base.h"

#include "log_parser_context.h"
#include "lexer.h"

#include <filesystem>
#include <memory>
#include <string>


struct log_data;
class log_parser_context;

class log_parser : public log_parser_base {

    lexer m_lexer;
    std::string const& m_matched;
    
    log_parser_context& m_ctx;

public:
    log_parser(std::filesystem::path const& path, log_parser_context& ctx);
    std::unique_ptr<log_data> gen();

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

inline int log_parser::lex()
{
    return m_lexer.lex();
}

inline void log_parser::print()
{
}
