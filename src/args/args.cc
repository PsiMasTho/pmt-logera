#include "args.h"
#include <regex>

using namespace std;

Args::Args(char const* optStr, char** argv)
:
	d_optTypeMap{},
	d_optValMap{}
{
	fillTypeMap(optStr);
	fillValMap(argv);
}

string const* Args::option(char option) const
{
		// check if the option is valid
	auto itr1 = d_optTypeMap.find(option);
	if (itr1 == d_optTypeMap.end())
		throw "Unknown option.";
	
	auto itr2 = d_optValMap.find(option);
	if (itr2 == d_optValMap.end()) // option not specified
		return nullptr;
	else
		return &itr2->second; // is specified, return string (may be empty)
		
}

void Args::fillTypeMap(string const& optStr)
{
	regex const optRegex(R"([a-z]\:{0,2})", regex::ECMAScript | regex::optimize | regex::icase);

	for (auto itr = sregex_iterator(optStr.begin(), optStr.end(), optRegex); itr != sregex_iterator(); ++itr)
	{                    
		smatch const match(*itr);
        
		char const optCh = match.str().front();
		Type optType;

		if (match.str().size() > 1)
		{
			string const optTypeStr(match.str().substr(1));
			if (optTypeStr == ":")
				optType = Type::REQUIRED;
			else if (optTypeStr == "::")
				optType = Type::OPTIONAL;
		}
		else
			optType = Type::NONE;

		d_optTypeMap[optCh] = optType;
    }   
}

void Args::fillValMap(char** argv)
{
	if (!argv[1])
    	return;

    vector<pair<char, string>> optValPairs;
    
    /* char		string
    ------------------------
        -arg1   "val1 val2 val3..."
        -arg2   "val1 val2 val3..."
        ...
    */

    for (int i = 1; argv[i]; ++i)
    {
        if (argv[i][0] == '-') // then opt
            optValPairs.push_back({argv[i][1], {}});
        else
		{
			if (optValPairs.empty())
				throw "Invalid arguments.";
            else
			{
				string const delim = optValPairs.back().second.empty() ? string{} : " "s;
				optValPairs.back().second += delim + string(&argv[i][0]);
			}
		}
    }

    for (auto [opt, val] : optValPairs)
	{
		if (d_optTypeMap.contains(opt))
		{
			Type const optType = d_optTypeMap[opt];
        	if (val.empty() && (optType == Type::NONE || optType == Type::OPTIONAL))
				d_optValMap[opt] = val;
			else if (!val.empty() && (optType == Type::OPTIONAL || optType == Type::REQUIRED))
				d_optValMap[opt] = val;
			else
				throw "Invalid arguments.";
		}
		else
			throw "Unknown option.";
	}
}
