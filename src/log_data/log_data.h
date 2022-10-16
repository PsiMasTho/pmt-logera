#ifndef INCLUDED_LOG_DATA_H
#define INCLUDED_LOG_DATA_H

#include "../date/date.h"
#include "../sparse_array/sparse_array.h"

#include <string>
#include <vector>

// class representing data of a single file
class LogData
{
public:
    // LogLine.get(0) == var name
    using LogLine = SparseArray<std::string>;

private:
    friend class LogDataModifier;

    Date d_date;
    std::vector<LogLine> d_lines;

public:
    LogData() = default;
    Date const& getDate() const;
    std::vector<LogLine> const& getLines() const;
};

#endif
