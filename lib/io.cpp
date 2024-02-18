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

auto readallf(char const* path, string& buffer, vector<error::record>& errors) -> string&
{
    ifstream file(path, ios::binary);

    if (!file.is_open())
    {
        errors.emplace_back(error::code::IO_CANNOT_OPEN_FILE, path);
        return buffer;
    }

    buffer.clear();
    buffer.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());

    if (file.fail())
    {
        errors.emplace_back(error::code::IO_CANNOT_READ_FILE, path);
        return buffer;
    }

    return buffer;
}

} // namespace io
