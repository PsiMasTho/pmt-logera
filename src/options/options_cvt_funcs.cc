#include "options.ih"

using namespace std;

// expects a single path to either a directory or a file, not checked if file exists
template <>
filesystem::path Options::cvtFunc<filesystem::path>(string const& valStr)
{
    return filesystem::path{valStr};
}

// expects one or more paths seperated by a single space, not checked if files exist
template <>
vector<filesystem::path> Options::cvtFunc<vector<filesystem::path>>(string const& valStr)
{
    vector<filesystem::path> ret;
    istringstream iss(valStr);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(ret));

    return ret;
}