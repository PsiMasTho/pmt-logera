#include "options.ih"

std::filesystem::path const& Options::headerFile() const
{
    return d_headerFile;
}

vector<filesystem::path> Options::logFiles() const
{
    return d_logFiles;
}

FilterType Options::filterType() const
{
    return d_filterType;
}

std::regex const& Options::filterRegex() const
{
    return d_filterRegex;
}