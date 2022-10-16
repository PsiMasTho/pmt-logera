#ifndef INCLUDED_DATE_H
#define INCLUDED_DATE_H

#include <chrono>
#include <optional>
#include <string>

class Date
{
    std::optional<std::chrono::year_month_day> d_date;

public:
    explicit Date(std::string const& dateStr);
    Date() = default;

    std::string to_string() const;
    bool ok() const;

    friend auto operator<=>(Date const&, Date const&) = default;
};

#endif
