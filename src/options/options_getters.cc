#include "options.ih"

filesystem::path const& Options::headerFile() const
{
    return d_headerFile;
}

std::filesystem::path const& Options::outputFile() const
{
    return d_outputFile;
}

vector<filesystem::path> const& Options::logFiles() const
{
    return d_logFiles;
}