#include "args.ih"

Args::Args(_Opt const* options, size_t optionCount, char const* const* argv)
    : d_optionToValueMap{}
{
    if(!argv[0])
        return;

    _Opt const magicOpt(_Opt(_Opt::ValType::REQUIRED, "\0", '\0'));

    // special key for program name
    if(auto ptr = createStringForOpt(magicOpt, true))
        *ptr = argv[0];

    _Opt target = magicOpt;

    for(size_t i = 1; argv[i]; ++i)
    {
        // if argv[i] is an option
        if(ArgType argtype = _argType(argv[i]); argtype != ArgType::VALUE)
        {
            size_t j = 0;
            for(; j < optionCount; ++j)
            {
                string const optStr =
                    argtype == LONG_OPT ? options[j].getName() : _toStr(options[j].getCh());

                // Skip one or two dashes
                auto const beg = argv[i] + (argtype == LONG_OPT ? 2 : 1);

                if(optStr == beg)
                {
                    target = (createStringForOpt(options[j], true) ? options[j] : magicOpt);
                    break;
                }
            }
            // The option the user passed doesn't match any valid option
            if(j == optionCount)
                throw invalid_argument("Unknown option: "s + string(argv[i]));
        }
        else // argv[i] must be a value
        {
            if(target == magicOpt && i == 1)
                throw invalid_argument("Value specified without option: "s + argv[i]);
            else if(target == magicOpt)
                continue;
            else if(target.getType() == _Opt::ValType::NONE)
                throw invalid_argument("Value specified for option taking to value");
            else
                _appendToString(
                    rawOption(target).second, argv[i], rawOption(target).second.empty() ? "" : " ");
        }
    }

    // Create an entry for every valid option that wasn't specified in argv.
    // createStringForOpt(...) will do nothing on already inserted elements,
    // so no checks are needed
    for(size_t j = 0; j < optionCount; ++j)
        createStringForOpt(options[j], false);
}

std::pair<bool, std::string const&> Args::argv0() const
{
    return option('\0');
}

std::pair<bool, std::string const&> Args::option(Args::key_t const& option) const
{
    return rawOption(option);
}

string* Args::createStringForOpt(_Opt const& opt, bool wasSpecified)
{
    shared_ptr<string> str = wasSpecified ? make_shared<string>() : nullptr;

    // {"\0", '\0'} AKA magicOpt will be occupied by argv0, thus emplacement will fail for every other Opt that tries to
    // use one of these as keys
    array<key_t, 2> const keys{opt.getCh(), opt.getName()};
    bool success = false;

    // Emplace keys/value pairs for short and long Args. success == true means at least one emplacement succeeded
    for(auto const& key : keys)
    {
        visit(
            [&](auto&& k) {
                success += d_optionToValueMap.emplace(k, make_pair(wasSpecified, str)).second;
            },
            key);
    }

    // if neither emplacement succeeded, then return a nullptr
    return success ? str.get() : nullptr;
}

pair<bool, string&> Args::rawOption(key_t const& option) const
{
    static string emptyStr;

    auto const itr = d_optionToValueMap.find(option);

    // throw an exception with the active field of the variant in the .what() message
    if(itr == d_optionToValueMap.end())
        throw invalid_argument("Nonexistant option queried: "s +
                               visit([](auto&& k) { return _toStr(k); }, option));

    // was specified
    if(itr->second.first)
        return {true, *itr->second.second};
    else
    {
        emptyStr.clear();
        return {false, emptyStr};
    }
}

pair<bool, string&> Args::rawOption(_Opt const& option) const
{
    if(_notNull(option.getCh()))
        return rawOption(option.getCh());
    else
        return rawOption(option.getName());
}

ArgType _argType(string_view arg)
{
    if(arg.size() == 2 && arg[0] == '-' && isalnum(arg[1]))
        return SHORT_OPT;
    else if(arg.size() > 2 && arg[0] == '-' && arg[1] == '-')
        return LONG_OPT;
    else
        return VALUE;
}

void _appendToString(string& lhs, string_view const rhs, string const& delim)
{
    lhs += delim;
    lhs += rhs;
}

string _toStr(char c)
{
    return string(1, c);
}

string _toStr(string const& str)
{
    return str;
}

bool _notNull(char c)
{
    return c;
}

bool _notNull(char const* c)
{
    return c;
}