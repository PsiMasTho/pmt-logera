//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "date.h"

#include <algorithm>
#include <cstdio>
#include <exception>
#include <sstream>
#include <utility>
#include <regex>
#include <tuple>

using namespace std;

namespace
{

chrono::year_month_day str_to_ymd(string const& ymd_str)
{
    static regex const ymd_regex(R"((\d{4})-(\d{2})-(\d{2}))", regex::optimize);
    smatch match;

    if (regex_match(ymd_str, match, ymd_regex)) {
        int const y = stoi(match[1]);
        int const m = stoi(match[2]);
        int const d = stoi(match[3]);

        auto const ret =
            chrono::year_month_day(chrono::year(y), chrono::month(m), chrono::day(d));
        if (ret.ok())
            return ret;
    }

    throw invalid_argument("Bad date string: "s + ymd_str);
}

tuple<int, unsigned, unsigned> split_ymd(chrono::year_month_day const& ymd)
{
    auto const yyyy = static_cast<int>(ymd.year());
    auto const mm = static_cast<unsigned>(ymd.month());
    auto const dd = static_cast<unsigned>(ymd.day());

    return {yyyy, mm, dd};
}

[[noreturn]] void throw_bad_ymd(chrono::year_month_day const& ymd)
{
    auto const [yyyy, mm, dd] = split_ymd(ymd);
    throw invalid_argument("Bad date: day: "s + to_string(dd) + " month: " + to_string(mm) +
                           " year: " + to_string(yyyy));
}

auto ymd_to_str(chrono::year_month_day ymd)
{
    auto const [yyyy, mm, dd] = split_ymd(ymd);

    array<char, DATE_STR_LEN + 1> ret;
    if (snprintf(ret.data(), ret.size(), "%04d-%02u-%02u", yyyy, mm, dd) != DATE_STR_LEN)
        throw_bad_ymd(ymd);

    return ret;
}

} // namespace

date::date(string const& date_str)
    : m_date(str_to_ymd(date_str))
    , m_str{ymd_to_str(m_date)}
{ }

char const* date::to_string() const
{
    return m_str.data();
}

strong_ordering operator<=>(date const& lhs, date const& rhs) noexcept
{
    return lhs.m_date <=> rhs.m_date;
}

bool operator==(date const& lhs, date const& rhs) noexcept
{
    return lhs.m_date == rhs.m_date;
}