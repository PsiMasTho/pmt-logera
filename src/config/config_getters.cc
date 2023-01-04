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