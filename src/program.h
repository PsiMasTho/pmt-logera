#pragma once

#include "config.h"
#include "header_data.h"
#include "input_error.h"
#include "log_data.h"

#include <filesystem>
#include <memory>
#include <stack>
#include <vector>

class program
{
    config m_cfg;
    std::stack<input_error> m_errors;
    std::unique_ptr<header_data> m_headerData;
    std::vector<std::unique_ptr<log_data>> m_log_data;

public:
    program(int argc, char** argv);
    int run();

private:
    void parse_header(std::filesystem::path const& path);
    void parse_log_files(std::vector<std::filesystem::path> const& paths);
    void print_errors();
    void generate_csv();
};