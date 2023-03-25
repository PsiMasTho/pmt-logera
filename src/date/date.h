//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_DATE_H
#define INCLUDED_DATE_H

#include <array>
#include <chrono>
#include <string>

// Represents a date in the form YYYY-MM-DD

class Date
{
    std::chrono::year_month_day d_date;
    std::array<char, 11> d_str;

public:
    explicit Date(std::string const& dateStr);
    Date() = default;

    char const* to_string() const;

    friend std::strong_ordering operator<=>(Date const&, Date const&) noexcept;
    friend bool operator==(Date const&, Date const&) noexcept;
};

#endif
