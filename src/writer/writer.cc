#include "writer.h"

#include <utility>

using namespace std;

namespace
{
// A field in an RFC 4180 CSV needs to
// be enclosed in double quotes under certain
// circumstances, for our format, we only
// need to check for a comma.
bool needsQuotes(string const& str)
{
    return str.find(',') != string::npos;
}
} // namespace

Writer::Writer(ostream& out)
    : d_out(out)
{ }

void Writer::write(Date const& date, LogData::LogLine const& logLine)
{
    d_out << date.to_string();
    for(size_t idx = 0; idx < logLine.capacity(); ++idx)
    {
        if(logLine.exists(idx))
        {
            if(needsQuotes(logLine.get(idx)))
                d_out << ",\"" << logLine.get(idx) << '"';
            else
                d_out << ',' << logLine.get(idx);
        }
        else
            d_out << ',';
    }

    d_out << '\n';
}

void Writer::write(std::vector<std::string> const& vec)
{
    string delim;
    for(auto const& str : vec)
        if(needsQuotes(str))
            d_out << exchange(delim, ",") << '"' << str << '"';
        else
            d_out << exchange(delim, ",") << str;

    d_out << '\n';
}
