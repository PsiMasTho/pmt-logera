//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "config.ih"

using namespace std;

// expects a single path to either a directory or a file, not checked if file exists
template <>
filesystem::path Config::cvtFunc<filesystem::path>(string const& valStr)
{
    return filesystem::path{valStr};
}

// expects one or more paths seperated by a single space, not checked if files exist
template <>
vector<filesystem::path> Config::cvtFunc<vector<filesystem::path>>(string const& valStr)
{
    vector<filesystem::path> ret;
    istringstream iss(valStr);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(ret));

    return ret;
}