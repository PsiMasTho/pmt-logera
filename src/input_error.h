#pragma once

#include <string>
#include <variant>

struct header_parse_error
{
    std::string filename;
    std::string msg;
    std::size_t line_nr;
};

struct log_parse_error
{
    std::string filename;
    std::string msg;
    std::size_t line_nr;
};

struct arg_parse_error
{
    std::string msg;
};

using input_error = std::variant<header_parse_error, log_parse_error, arg_parse_error>;