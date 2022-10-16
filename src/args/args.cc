#include "args.ih"

using namespace std;

namespace
{
// maps lowercase and uppercase chars to range [0, 52],
// expects a valid lowercase or uppercase ascii char
size_t mapCh(char ch);
} // namespace

Args::Args(char const* optNTBS, char const* const* argv)
    : d_types(52)
    , d_vals(52)
{
    setTypes(optNTBS);
    setVals(argv);
}

std::string const& Args::option(bool* wasSpecified, char option) const
{
    if(!d_vals.exists(mapCh(option))) // option not specified
    {
        *wasSpecified = false;

        static string emptyRet{};
        return emptyRet;
    }
    else
    {
        *wasSpecified = true;
        return d_vals.get(mapCh(option)); // is specified, return string (may be empty)
    }
}

void Args::setTypes(char const* optNTBS)
{
    static regex const optRegex(R"([a-z]\:{0,2})",
                                regex::ECMAScript | regex::optimize | regex::icase);
    size_t const ntbsLen = strlen(optNTBS);

    if(!ntbsLen)
        return;

    if(cmatch optMatch; regex_search(optNTBS, optMatch, optRegex))
    {
        char const ch = optMatch[0].str()[0];
        size_t const matchLen = optMatch[0].length();
        d_types.set(mapCh(ch), static_cast<ValType>(matchLen));
        setTypes(optNTBS + matchLen);
    }
}

void Args::setVals(char const* const* argv)
{
    if(!argv[1])
        return;

    vector<pair<char, string>> optValPairs;

    /* char        string
  ------------------------
      -arg1   "val1 val2 val3..."
      -arg2   "val1 val2 val3..."
      ...
  */

    for(int i = 1; argv[i]; ++i)
    {
        if(argv[i][0] == '-') // then opt
            optValPairs.push_back({argv[i][1], {}});
        else
        {
            if(optValPairs.empty())
                throw invalid_argument{{}};
            else
            {
                string const delim = optValPairs.back().second.empty() ? string{} : " "s;
                optValPairs.back().second += delim + string(&argv[i][0]);
            }
        }
    }

    for(auto [opt, val] : optValPairs)
    {
        if(d_types.exists(mapCh(opt)))
        {
            uint8_t const optType = d_types.get(mapCh(opt));
            if(val.empty() && (optType == ValType::WITHOUT || optType == ValType::OPTIONAL))
                d_vals.set(mapCh(opt), val);
            else if(!val.empty() && (optType == ValType::OPTIONAL || optType == ValType::REQUIRED))
                d_vals.set(mapCh(opt), val);
            else if(val.empty() && optType == ValType::REQUIRED)
                throw invalid_argument("No value specified for argument: "s + '-' + opt);
            else if(!val.empty() && optType == ValType::WITHOUT)
                throw invalid_argument("Value specified for argument: "s + '-' + opt);
        }
        else
            throw invalid_argument("Unknown option: "s + opt);
    }
}

namespace
{
size_t mapCh(char ch)
{
    if(isupper(ch))
        return ch - 65;
    else
        return ch - 97;
}
} // namespace