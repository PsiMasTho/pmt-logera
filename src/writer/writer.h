#ifndef INCLUDED_WRITER_H
#define INCLUDED_WRITER_H

#include "../log_data/log_data.h"
#include <iosfwd>
#include <string>
#include <functional>

class Writer
{
    LogData const& d_data;
    std::ostream&  d_out;
    std::function<bool(std::string const&)> d_identFilter;

public:
    Writer(LogData const& data, std::ostream& out, std::function<bool(std::string const&)> identFilter);
    void write();
};

#endif