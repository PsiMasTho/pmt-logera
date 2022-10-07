#include "date.ih"

Date::Date(string const& dateStr)
:
    d_date(_strToYMD(dateStr))
{
    if (!ok())
        throw "Bad date string: "s + dateStr;
}

string Date::to_string() const
{
    if (!ok())
        throw "Invalid date"s;
    else
        return _ymdToStr(d_date.value());
}

bool Date::ok() const
{
    return d_date.has_value() && d_date.value().ok();
}