#include "writer.h"

#include <utility>

using namespace std;

Writer::Writer(filesystem::path const& outfile, char delim)
    : d_out(outfile)
    , d_delim{delim}
{ }

void Writer::write(Date const& date, LogData::LogLine const& logLine)
{
    d_out << date.to_string();
    for(size_t idx = 0; idx < logLine.capacity(); ++idx)
    {
        if(logLine.exists(idx))
            d_out << d_delim << logLine.get(idx);
        else
            d_out << d_delim;
    }

    d_out << '\n';
}

void Writer::write(std::vector<std::string> const& vec)
{
    string delim;
    for(auto const& entry : vec)
        d_out << exchange(delim, string(1, d_delim)) << entry;
    d_out << '\n';
}
