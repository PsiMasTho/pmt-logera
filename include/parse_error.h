#pragma once

#include <string>
#include <cstddef>

struct parse_error
{
    std::string filename;
    std::string msg;
    std::size_t line_nr;
};