#ifndef INCLUDED_ARG_H
#define INCLUDED_ARG_H
/*
*    A simplified version of the Arg class found in lib bobcat
*/

#include <unordered_map>
#include <string>
#include <optional>

class Args
{
    enum Type
    {
        NONE,
        REQUIRED,
        OPTIONAL
    };

    std::unordered_map<char, Type>        d_optTypeMap;
    std::unordered_map<char, std::string> d_optValMap;

public:
    Args(char const* optStr, char** argv);

        // returns nullptr if option isnt specified, returns empty string if it is specified
        // with no arguments, returns filled string if it's specified and has arguments
    std::string const* option(char option) const;

private:
    void fillTypeMap(std::string const& optStr);
    void fillValMap(char** argv);
};

#endif
