#include "args.h"
#include <memory>

using namespace std;

Args::Args(char** argv)
:
    d_argMap{}
{
    registerArgs();
    loadArgv(argv);
}

void Args::registerArgs()
{
    #define CLI_ARG_OPT(letter, name, Type, unused) registerArg<Type>(d_ ## name, #name); addArg(letter, #name);
    #include "arg_list.h"
}

void Args::addArg(char letter, string const& name)
{
    d_argMap[letter] = name;
}

void Args::loadArgv(char** argv)
{
        // no arguments passed (only program name)
    if (!argv[1])
        return;

    vector<pair<char, string>> argValPairs;
    
    /* <char>   <string>
    ------------------------
        -a   "value string 1..."
        -b   "value string 2..."
        ...
    */

    for (size_t i = 1; argv[i]; ++i)
    {
        if (argv[i][0] == '-') // then opt
            argValPairs.push_back({argv[i][1], {}});
        else
            argValPairs.back().second += string(&argv[i][0]) + " "s;
    }

    // remove trailing space
    for (auto &[arg, val] : argValPairs)
        if (!val.empty())
            val.pop_back();

    for (auto [arg, val] : argValPairs)
        set(d_argMap[arg], val);
}