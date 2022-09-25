#include "main.ih"

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
    RegexFilterBase* filter = 
        (!args.d_inclusiveFilter.has_value() && !args.d_exclusiveFilter.has_value()) ? nullptr :
        args.d_inclusiveFilter.has_value() ? dynamic_cast<RegexFilterBase*>(&args.d_inclusiveFilter.value()) : dynamic_cast<RegexFilterBase*>(&args.d_exclusiveFilter.value());

    if (filter)
        cout << "accept? " << (filter->accept("hello") ? "YES!" : "NO!") << endl;

    HeaderParser headerParser();

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