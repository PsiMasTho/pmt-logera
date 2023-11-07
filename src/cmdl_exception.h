#pragma once

#include <stdexcept>
#include <string>

struct cmdl_exception : public std::invalid_argument
{
    explicit cmdl_exception(std::string const& what) : std::invalid_argument(what) {}
};