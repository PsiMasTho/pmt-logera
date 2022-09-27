#include "main.ih"

#include "scanner/scanner.h"

enum
{
    SUCCESS = 0,
    FAIL
};

bool validateArgs(Args& args);

int run(Args& args)
{
    if (validateArgs(args) != SUCCESS)
        return FAIL;

        // set the filter, or nullptr if there is none
    std::function<bool(std::string)> filter;
    if (args.d_inclusiveFilter.has_value())
        filter = [&args](std::string str){return regex_match(str, args.d_inclusiveFilter.value());};
    else if (args.d_exclusiveFilter.has_value())
        filter = [&args](std::string str){return !regex_match(str, args.d_exclusiveFilter.value());};
    else
        filter = [](std::string){return true;};

    cout << "accept? " << (filter("hello") ? "YES!" : "NO!") << endl;

    return SUCCESS;
}

bool validateArgs(Args& args)
{
    if (args.d_inclusiveFilter.has_value() && args.d_exclusiveFilter.has_value())
    {
        cerr << "Error: cannot specify -E flag and -I flag together.\n";
        return FAIL;
    }

    return SUCCESS;
}