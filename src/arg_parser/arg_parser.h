#ifndef INCLUDED_ARG_PARSER_H
#define INCLUDED_ARG_PARSER_H

#include "../args/args.h"

struct DefaultArgParser : public IArgParser<DefaultArgParser>
{
    using IArgParser<DefaultArgParser>::IArgParser;
    std::pair<std::variant<char, std::string>, std::string> next();
};

#endif