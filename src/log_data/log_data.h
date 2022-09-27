#ifndef INCLUDED_CSV_DATA_H
#define INCLUDED_CSV_DATA_H

#include "log_line.h"
#include <iosfwd>
#include <functional>

class LogData
{
    std::vector<OutputLine> d_lines;

public:
    Writer(std::ostream& out, std::function<bool(std::string const&)> identFilter);
    
};

#endif