#ifndef INCLUDED_WRITER_H
#define INCLUDED_WRITER_H

#include "../log_data/log_data.h"
#include <fstream>
#include <string>
#include <functional>
#include <filesystem>

class LogLine;

class Writer
{
    std::ofstream                           d_out;
    std::function<bool(std::string const&)> d_identFilter;

public:
    Writer(std::filesystem::path const& outfile, std::function<bool(std::string const&)> identFilter);
    void write(Date const& date, LogLine const& logLine);
};

#endif