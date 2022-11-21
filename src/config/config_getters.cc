#include "config.ih"

decltype(Config::d_verbose) const& Config::verbose() const
{
    return d_verbose;
}

decltype(Config::d_headerFile) const& Config::headerFile() const
{
    return d_headerFile;
}

decltype(Config::d_outputFile) const& Config::outputFile() const
{
    return d_outputFile;
}

decltype(Config::d_logFiles) const& Config::logFiles() const
{
    return d_logFiles;
}