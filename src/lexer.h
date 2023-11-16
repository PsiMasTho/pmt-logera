#pragma once

#include "lexer_base.h"
#include "tokens.h"

#include <string>
#include <cstring>

class lexer: public lexer_base
{
    public:
        explicit lexer(std::string const& infile);
        int lex();

    private:
        int lex_();
        int executeAction_(size_t ruleNr);

        void print();
        void preCode();     // re-implement this function for code that must 
                            // be exec'ed before the patternmatching starts

        void postCode(PostEnum_ type);    
                            // re-implement this function for code that must 
                            // be exec'ed after the rules's actions.
};

// $insert scannerConstructors
inline lexer::lexer(std::string const& infile)
:
    lexer_base(infile, "-")
{}

// $insert inlineLexFunction
inline int lexer::lex()
{
    return lex_();
}

inline void lexer::preCode() 
{
    // optionally replace by your own code
}

inline void lexer::postCode([[maybe_unused]] PostEnum_ type) 
{
    // optionally replace by your own code
}

inline void lexer::print() 
{
    print_();
}



