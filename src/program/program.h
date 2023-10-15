#ifndef INCLUDED_PROGRAM_PROGRAM_H
#define INCLUDED_PROGRAM_PROGRAM_H

#include "../config/config.h"
#include "../header_data/header_data.h"
#include "../header_parser/header_parser.h"
#include "../log_data/log_data.h"
#include "../log_parser/log_parser.h"
#include "../input_error/input_error.h"

#include <stack>
#include <memory>
#include <vector>
#include <filesystem>
#include <iosfwd>

class program
{
    Config m_cfg;
    std::stack<input_error> m_errors;
    std::unique_ptr<header_data> m_headerData;
    std::vector<std::unique_ptr<LogData>> m_logData;

public:
    program(int argc, char** argv);
    int run();

private:
    void parse_header(std::filesystem::path const& path);
    void parse_log_files(std::vector<std::filesystem::path> const& paths);
    void print_errors();
    void generate_csv();
};

#endif