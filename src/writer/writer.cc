#include "writer.h"

#include <utility>

using namespace std;

Writer::Writer(filesystem::path const& outfile, function<bool(string const&)> identFilter, std::string const& delim)
:
    d_out(outfile),
    d_identFilter(identFilter),
    d_delim{delim}
{}

void Writer::write(Date const& date, LogData::LogLine const& logLine)
{
        // only write if the filter accepts
    if (!d_identFilter(logLine.first))
        return;

    d_out << date.to_string() << ";" << logLine.first;
    for (auto const& val : logLine.second)
        d_out << d_delim << val;
    d_out << '\n';
}

void Writer::write(std::vector<std::string> const& vec)
{
    string delim;
    for (auto const& entry : vec)
        d_out << exchange(delim, d_delim) << entry;
    d_out << '\n';
}
