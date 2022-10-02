#ifndef INCLUDED_DATE_H
#define INCLUDED_DATE_H

#include <chrono>
#include <string>
#include <iosfwd>

class Date
{
    friend bool operator<(Date const& lhs, Date const& rhs);

    std::chrono::year_month_day d_date;
    std::string                 d_string;

public:
    explicit Date(std::string const& dateStr);

    std::string const& to_string() const;
};

bool operator<(Date const& lhs, Date const& rhs);

#endif
