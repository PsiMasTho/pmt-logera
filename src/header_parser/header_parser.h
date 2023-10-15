// Generated by Bisonc++ V6.04.04 on Fri, 07 Oct 2022 23:06:16 +0200

#ifndef HeaderParser_h_included
#define HeaderParser_h_included

// $insert baseclass
#include "header_parserbase.h"
// $insert scanner.h
#include "../header_scanner/header_scanner.h"

#include "../header_data/header_data.h"
#include "../input_error/input_error.h"
#include <filesystem>
#include <fstream>
#include <iosfwd>
#include <optional>

class header_parser : public header_parser_base {

    // $insert scannerobject
    HeaderScanner d_scanner;
    std::string const& d_matched;
    std::unique_ptr<header_data> d_ret;
    std::optional<input_error> d_errorInfo;

public:
    explicit header_parser(std::filesystem::path const &path);
    int parse();
    std::unique_ptr<header_data> gen();
    input_error const& getErrorInfo() const;

private:
    void error(); // called on (syntax) errors
    int  lex(); // returns the next token from the
        // lexical scanner.
    void print(); // use, e.g., d_token, d_loc
    void exceptionHandler(std::exception const& exc);

    // support functions for parse():
    void executeAction_(int ruleNr);
    void errorRecovery_();
    void nextCycle_();
    void nextToken_();
    void print_();
};

#endif
