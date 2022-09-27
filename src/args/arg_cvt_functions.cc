#include "args.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <regex>

using namespace std;

    // expects a single path to either a directory or a file, not checked if file exists
template <>
filesystem::path Args::cvtFunc<filesystem::path>(string const& valStr)
{
    filesystem::path ret(valStr);

    return ret;
}

    // expects one or more paths seperated by a single space, not checked if files exist
template <>
vector<filesystem::path> Args::cvtFunc<vector<filesystem::path>>(string const& valStr)
{
    vector<filesystem::path> ret;
    istringstream iss(valStr);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(ret));
    
    return ret;
}

template <>
regex Args::cvtFunc<regex>(string const& valStr)
{
    return regex(valStr);
}