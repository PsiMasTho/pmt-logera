//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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