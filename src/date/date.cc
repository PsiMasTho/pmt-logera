#include "date.ih"

Date::Date(string const& dateStr)
:
    d_date(_strToYMD(dateStr))
{
    if (!d_date.ok())
        throw "Bad date string";
}

string Date::to_string() const
{
    return _ymdToStr(d_date);
}