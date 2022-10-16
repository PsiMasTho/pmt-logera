#include "options.ih"

decltype(Options::d_verbose) const& Options::verbose() const
{
    return d_verbose;
}

decltype(Options::d_headerFile) const& Options::headerFile() const
{
    return d_headerFile;
}

decltype(Options::d_outputFile) const& Options::outputFile() const
{
    return d_outputFile;
}

decltype(Options::d_logFiles) const& Options::logFiles() const
{
    return d_logFiles;
}