//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "log_data.h"
#include "header_data.h"

#include <algorithm>

using namespace std;

date const& log_data::get_date() const
{
    return m_date;
}

std::vector<log_data::log_line> const& log_data::get_lines() const
{
    return m_lines;
}