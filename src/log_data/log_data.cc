#include "log_data.h"

#include "../header_data/header_data.h"

#include <algorithm>

using namespace std;

Date const& LogData::getDate() const
{
    return d_date;
}

std::vector<LogData::LogLine> const& LogData::getLines() const
{
    return d_lines;
}