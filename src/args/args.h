#ifndef INCLUDED_ARGS_H
#define INCLUDED_ARGS_H

#include "../args_base/args_base.h"
#include <string>
#include <unordered_map>

    // begin headers for "arg_list.h"
#include <vector>
#include <filesystem>
#include <regex>
    // end headers for "arg_list.h"

class Args : public ArgsBase<Args>
{
        // maps arg letters to variable names e.g. arg -f: {f, filenames}
    std::unordered_map<char, std::string> d_argMap;

public:
    Args(char** argv);
    Args(Args const&) = default;
    Args(Args&&) =      default;
    ~Args() =           default;

    #define CLI_ARG_OPT(unused0, name, Type, defaultVal) std::optional< Type > d_ ## name = defaultVal;
    #include "arg_list.h"

    template <typename Type>
    static Type cvtFunc(std::string const& valStr);

private:
    void registerArgs();
    void addArg(char letter, std::string const& name);
    void loadArgv(char** argv);
};

#endif