//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "config.ih"

bool Config::verbose() const
{
    return d_verbose;
}

filesystem::path const& Config::headerFile() const
{
    return d_headerFile;
}

ostream& Config::outputStream()
{
    return d_outputStream ? *d_outputStream : std::cout;
}

vector<filesystem::path> const& Config::logFiles() const
{
    return d_logFiles;
}