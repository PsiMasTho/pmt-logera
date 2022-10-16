#ifndef INCLUDED_WRITER_H
#define INCLUDED_WRITER_H

#include "../log_data/log_data.h"
#include <fstream>
#include <string>
#include <functional>
#include <filesystem>

class LogLine;

class Writer {
    std::ofstream d_out;
    std::string   d_delim;

public:
    Writer(std::filesystem::path const& outfile, std::string const& delim);
    void write(Date const& date, LogData::LogLine const& logLine);
    void write(std::vector<std::string> const& vec);
};

#endif