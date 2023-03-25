//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "args.ih"

namespace
{
string const emptyStr;

bool flagMatch(Args::Opt::flag_t const& optFlags, variant<char, string> const& flag)
{
    if(optFlags.index() == 2)
    {
        if(flag.index() == 0)
            return get<0>(flag) == get<2>(optFlags).first; // compare chars
        return get<1>(flag) == get<2>(optFlags).second; // compare str
    }
    else if(optFlags.index() == 1)
    {
        if(flag.index() == 1)
            return get<1>(flag) == get<1>(optFlags); // compare strings
        return false;
    }
    else
    {
        if(flag.index() == 0)
            return get<0>(flag) == get<0>(optFlags); // compare chars
        return false;
    }
}

string to_string(variant<char, string> const& flag)
{
    return visit(
        [](auto const& flg) -> string {
            ostringstream ret;
            ret << flg;
            return ret.str();
        },
        flag);
}

} // namespace

char const* Args::argv0() const
{
    return d_argv0;
}

pair<bool, string const&> Args::option(variant<char, string> option) const
{
    /* Option was known and seen in argv */
    // Holds char
    if(holds_alternative<char>(option))
    {
        if(auto const itr = d_chFlagToValIdx.find(get<char>(option)); itr != end(d_chFlagToValIdx))
            return {true, d_vals[itr->second]};
    }
    // Holds string
    else if(auto const itr = d_strFlagToValIdx.find(get<string>(option));
            itr != end(d_strFlagToValIdx))
        return {true, d_vals[itr->second]};

    /* Option was known but not seen in argv */
    for(auto const& opt : d_unseenOpts)
        if(flagMatch(opt.getFlags(), option))
            return {false, emptyStr};

    /* Unknown option queried */
    throw invalid_argument("Unknown option queried: "s + to_string(option));
}

void Args::addOption(variant<char, string> const& flag, string const& val)
{
    auto const itr = find_if(d_unseenOpts.begin(), d_unseenOpts.end(), [flag](auto const& opt) {
        return flagMatch(opt.getFlags(), flag);
    });

    if(itr == d_unseenOpts.end())
        throw invalid_argument("Unknown or duplicated option: "s + to_string(flag));

    if(!val.empty())
    {
        // Check if valtype is optional or necessary
        if(itr->getValType() == ValType::OPTIONAL || itr->getValType() == ValType::REQUIRED)
        {
            if(visit([this](auto const& flg) { return !option(flg).first; }, flag))
                d_vals.emplace_back(val);
        }
        else
            throw invalid_argument(string("Value specified for option taking no value: ") + val);
    }
    else
    {
        if(itr->getValType() != ValType::REQUIRED)
        {
            if(visit([this](auto const& flg) { return !option(flg).first; }, flag))
                d_vals.emplace_back();
        }
        else
            throw invalid_argument("No value provided for flag: "s + to_string(flag));
    }

    // Add the index of the value to the lookup table(s)
    switch(itr->getFlags().index())
    {
    case 0: // This flag only has a char
        d_chFlagToValIdx[get<char>(itr->getFlags())] = d_vals.size() - 1;
        break;
    case 1: // This flag only has a string
        d_strFlagToValIdx[get<string>(itr->getFlags())] = d_vals.size() - 1;
        break;
    case 2: // This flag has a char and a string
        d_chFlagToValIdx[get<2>(itr->getFlags()).first] = d_vals.size() - 1;
        d_strFlagToValIdx[get<2>(itr->getFlags()).second] = d_vals.size() - 1;
        break;
    }

    // We have now seen the option
    d_unseenOpts.erase(itr);
}