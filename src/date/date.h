#ifndef INCLUDED_DATE_H
#define INCLUDED_DATE_H

#include <chrono>
#include <string>
#include <iosfwd>

class Date
{
    friend bool operator<(Date const& lhs, Date const& rhs);
    std::chrono::year_month_day d_date;
    std::string const			d_string;

public:
    explicit Date(std::string const& dateStr);
    Date(Date const&) = default;
    Date(Date&&) = default;
    ~Date() = default;

    std::string const& to_string() const;
};

bool operator<(Date const& lhs, Date const& rhs);

#endif