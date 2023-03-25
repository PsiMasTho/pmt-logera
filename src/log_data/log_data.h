//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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
