#ifndef INCLUDED_WRITER_H
#define INCLUDED_WRITER_H

#include "../log_data/log_data.h"
#include <filesystem>
#include <fstream>
#include <string>

class LogLine;

// Writes log lines or vectors of strings to a
// file in CSV format according to RFC 4180.
class Writer
{
    std::ostream& d_out;
    char d_delim;

public:
    Writer(std::ostream& out);
    void write(Date const& date, LogData::LogLine const& logLine);
    void write(std::vector<std::string> const& vec);
};

#endif