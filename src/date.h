//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <array>
#include <string>

// Represents a date in the form YYYY-MM-DD

enum
{
    DATE_STR_LEN = 10 // not including null terminator
};

class date
{
    std::array<char, DATE_STR_LEN + 1> m_str;

public:
    explicit date(std::string const& date_str);
    date() = default;

    char const* to_string() const;

    friend std::strong_ordering operator<=>(date const&, date const&) noexcept;
    friend bool operator==(date const&, date const&) noexcept;
};
