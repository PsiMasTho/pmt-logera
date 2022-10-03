#ifndef INCLUDED_DATE_H
#define INCLUDED_DATE_H

#include <chrono>
#include <string>

class Date
{
    std::chrono::year_month_day d_date;

public:
    explicit Date(std::string const& dateStr);

    std::string to_string() const;

    friend auto operator<=>(Date const&, Date const&) = default;
};

#endif
