#include "writer.h"

#include <utility>

using namespace std;

Writer::Writer(filesystem::path const& outfile, std::string const& delim)
:
    d_out(outfile),
    d_delim{delim}
{}

void Writer::write(Date const& date, LogData::LogLine const& logLine)
{
    d_out << date.to_string() << ";";
    for (auto const& val : logLine)
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
