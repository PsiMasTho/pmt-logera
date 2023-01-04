#ifndef INCLUDED_WRITER_H
#define INCLUDED_WRITER_H

#include "../log_data/log_data.h"
#include <filesystem>
#include <fstream>
#include <string>

class LogLine;

class Writer
{
    std::ostream& d_out;
    char d_delim;

public:
    Writer(std::ostream& out, char delim);
    void write(Date const& date, LogData::LogLine const& logLine);
    void write(std::vector<std::string> const& vec);
};

#endif