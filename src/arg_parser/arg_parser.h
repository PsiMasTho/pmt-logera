#ifndef INCLUDED_ARG_PARSER_H
#define INCLUDED_ARG_PARSER_H

#include "../args/args.h"

struct DefaultArgParser : ArgParserCore
{
    DefaultArgParser(char const* const* argv)
        : ArgParserCore{argv}
    { }

    std::pair<std::variant<char, std::string>, std::string> next();
    bool done() const;
};

#endif