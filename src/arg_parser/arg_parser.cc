#include "arg_parser.h"

#include <algorithm>
#include <sstream>

using namespace std;

enum Token
{
    SHORT_OPT,
    LONG_OPT,
    INVALID_OPT, // e.g. ---foo
    VALUE
};

// Checks whether an argument is a short option (e.g. -a), long option (e.g. --foo) or a value (e.g. 123abc)
static Token categorize(string_view arg)
{
    if(arg.size() == 2 && arg[0] == '-' && isalnum(arg[1]))
        return SHORT_OPT;
    else if(arg.size() > 2 && arg[0] == '-' && arg[1] == '-' && arg[2] != '-')
        return LONG_OPT;
    else if(arg.size() > 2 && arg[0] == '-' && arg[1] == '-' && arg[2] == '-')
        return INVALID_OPT;
    else
        return VALUE;
}

pair<variant<char, string>, string> DefaultArgParser::next()
{
    pair<variant<char, string>, string> ret;

    size_t itr = 0;
    bool hasFlag = false;

    do
    {
        switch(categorize(*d_argv))
        {
        case Token::SHORT_OPT: {
            // skip one dash and assign flag char
            ret.first = d_argv[0][1];
            hasFlag = true;
            break;
        }
        case Token::LONG_OPT: {
            // skip two dashes and assign flag string
            ret.first = &d_argv[0][2];
            hasFlag = true;
            break;
        }
        case Token::VALUE: {
            // keep building the value
            ret.second += (itr > 1 ? " "s : ""s) + string(*d_argv);
            break;
        }
        case Token::INVALID_OPT:
        default: {
            throw invalid_argument("Unexpected argument: "s + string(*d_argv));
        }
        }
        ++d_argv;
        ++itr;
    } while(*d_argv && categorize(*d_argv) != Token::SHORT_OPT &&
            categorize(*d_argv) != Token::LONG_OPT);

    if(!hasFlag)
        throw invalid_argument("Value has no flag: "s + ret.second);
    else
        return ret;
}

bool DefaultArgParser::done() const
{
    return !*d_argv;
}
