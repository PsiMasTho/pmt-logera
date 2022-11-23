#ifndef INCLUDED_DATE_H
#define INCLUDED_DATE_H

#include <array>
#include <chrono>
#include <string>

// Represents a date dd/mm/yyyy

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
