//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "date.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstring>
#include <stdexcept>

using namespace std;

namespace
{

[[noreturn]] void throw_bad_ymd(string const& ymd_str)
{
    throw invalid_argument("Bad date string: "s + ymd_str);
}

bool validate_ymd_str(string const& ymd_str)
{
    auto const is_delim_idx = [](size_t idx) { return idx == 4 || idx == 7; };

    if (ymd_str.size() != DATE_STR_LEN)
        return false;
    
    for (size_t idx = 0; idx < ymd_str.size(); ++idx)
    {
        if (is_delim_idx(idx))
        {
            if (ymd_str[idx] != '-')
                return false;
        }
        else
        {
            if (!isdigit(ymd_str[idx]))
                return false;
        }
    }
    
    return true;
}

bool validate_ymd_date(string const& ymd_str)
{
    int const yyyy = stoi(ymd_str.substr(0, 4));
    unsigned const mm = stoul(ymd_str.substr(5, 2));
    unsigned const dd = stoul(ymd_str.substr(8, 2));

    auto const ymd = chrono::year_month_day(chrono::year(yyyy), chrono::month(mm), chrono::day(dd));
    return ymd.ok();
}

bool validate_ymd(string const& ymd_str)
{
    return validate_ymd_str(ymd_str) && validate_ymd_date(ymd_str);
}

} // namespace

date::date(string const& ymd_str)
    : m_str{}
{
    if (!validate_ymd(ymd_str))
        throw_bad_ymd(ymd_str);
    copy_n(ymd_str.data(), DATE_STR_LEN + 1, m_str.data());
}

char const* date::to_string() const
{
    return m_str.data();
}

strong_ordering operator<=>(date const& lhs, date const& rhs) noexcept
{
    return strncmp(lhs.to_string(), rhs.to_string(), DATE_STR_LEN) <=> 0;
}

bool operator==(date const& lhs, date const& rhs) noexcept
{
    return (lhs <=> rhs) == 0;
}