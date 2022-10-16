#include "date.ih"

Date::Date(string const& dateStr)
:
    d_date(_strToYMD(dateStr))
{
    if (!ok())
        throw invalid_argument("Bad date string: "s + dateStr);
}

string Date::to_string() const
{
    return _ymdToStr(d_date.value());
}

bool Date::ok() const
{
    return d_date.has_value() && d_date.value().ok();
}