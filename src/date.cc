//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "date.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <exception>
#include <utility>
#include <regex>
#include <tuple>

using namespace std;

namespace
{

[[noreturn]] void throw_bad_ymd(string const& ymd_str);
[[noreturn]] void throw_bad_ymd(chrono::year_month_day const& ymd);
tuple<int, unsigned, unsigned> split_ymd(string const& ymd_str);
tuple<int, unsigned, unsigned> split_ymd(chrono::year_month_day const& ymd);
array<char, DATE_STR_LEN + 1> ymd_to_str(chrono::year_month_day ymd);
chrono::year_month_day str_to_ymd(string const& ymd_str);

} // namespace

date::date(string const& date_str)
    : m_str{ymd_to_str(str_to_ymd(date_str))} // the nested functions perform the validation
{ }

char const* date::to_string() const
{
    return m_str.data();
}

strong_ordering operator<=>(date const& lhs, date const& rhs) noexcept
{
    return str_to_ymd(lhs.to_string()) <=> str_to_ymd(rhs.to_string());
}

bool operator==(date const& lhs, date const& rhs) noexcept
{
    return strncmp(lhs.to_string(), rhs.to_string(), DATE_STR_LEN) == 0;
}

namespace
{

[[noreturn]] void throw_bad_ymd(string const& ymd_str)
{
    throw invalid_argument("Bad date string: "s + ymd_str);
}

[[noreturn]] void throw_bad_ymd(chrono::year_month_day const& ymd)
{
    auto const [yyyy, mm, dd] = split_ymd(ymd);
    throw invalid_argument("Bad date: year: "s + to_string(yyyy) + " month: " + to_string(mm) +
                            " day: " + to_string(dd));
}

tuple<int, unsigned, unsigned> split_ymd(string const& ymd_str)
{
    static regex const ymd_regex(R"((\d{4})-(\d{2})-(\d{2}))", regex::optimize);
    smatch match;

    if (!regex_match(ymd_str, match, ymd_regex))
        throw_bad_ymd(ymd_str);
    
    return {stoi(match[1]), stoul(match[2]), stoul(match[3])};
    
}

tuple<int, unsigned, unsigned> split_ymd(chrono::year_month_day const& ymd)
{
    auto const yyyy = static_cast<int>(ymd.year());
    auto const mm = static_cast<unsigned>(ymd.month());
    auto const dd = static_cast<unsigned>(ymd.day());

    return {yyyy, mm, dd};
}

array<char, DATE_STR_LEN + 1> ymd_to_str(chrono::year_month_day ymd)
{
    array<char, DATE_STR_LEN + 1> ret;

    auto const [yyyy, mm, dd] = split_ymd(ymd);
    if (snprintf(ret.data(), ret.size(), "%04d-%02u-%02u", yyyy, mm, dd) != DATE_STR_LEN)
        throw_bad_ymd(ymd);

    return ret;
}

chrono::year_month_day str_to_ymd(string const& ymd_str)
{
    auto const [yyyy, mm, dd] = split_ymd(ymd_str);

    auto const ret = chrono::year_month_day(chrono::year(yyyy), chrono::month(mm), chrono::day(dd));
    if (!ret.ok())
        throw_bad_ymd(ret);
    
    return ret;

}

}