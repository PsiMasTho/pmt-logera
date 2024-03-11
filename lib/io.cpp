//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/io.hpp"

#include <fstream>

using namespace std;

namespace io
{

auto basename_from_path(string_view const path) -> string_view
{
    auto const pos = path.find_last_of("/\\");

    if (pos == string_view::npos)
        return path;

    return path.substr(pos + 1);
}

auto readallf(char const* path, string& buffer, error::container& errors) -> bool
{
    ifstream file(path, ios::binary);

    if (!file.is_open())
    {
        errors.emplace_back(error::make_record<error::cannot_open_file>(path));
        return false;
    }

    buffer.clear();

    try
    {
        buffer.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    }
    catch (exception const& e)
    {
        errors.emplace_back(error::make_record<error::exception_reading_file>(path, e.what()));
        return false;
    }

    if (file.fail())
    {
        errors.emplace_back(error::make_record<error::failed_reading_file>(path));
        return false;
    }

    return true;
}

} // namespace io
