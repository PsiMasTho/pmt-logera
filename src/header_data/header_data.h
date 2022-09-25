#ifndef INCLUDED_HEADER_DATA_H
#define INCLUDED_HEADER_DATA_H

#include <string>
#include <unordered_map>
#include <utility>

    // fwd declare
enum FormatSpecifier : int;

class HeaderData
{
        // {ident, {fmt, fmt}}
    std::unordered_map<std::string, std::pair<FormatSpecifier, FormatSpecifier>> d_taskData;
        // {ident, fmt}
    std::unordered_map<std::string, FormatSpecifier> d_varData;

public:
    void addTask(std::string const& ident, FormatSpecifier extent, FormatSpecifier intensity);
    void addVariable(std::string const& ident, FormatSpecifier fmt);

    bool checkExercise(std::string const& ident, std::string const& extent, std::string const& intensity);
    bool checkVariable(std::string const& name, std::string const& fmt);
};

#endif