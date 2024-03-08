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
    buffer.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());

    if (file.fail())
    {
        errors.emplace_back(error::make_record<error::failed_reading_file>(path));
        return false;
    }

    return true;
}

} // namespace io
