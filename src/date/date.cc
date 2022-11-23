#include "date.ih"

Date::Date(string const& dateStr)
    : d_date(_strToYMD(dateStr))
    , d_str(_ymdToStr(d_date))
{ }

char const* Date::to_string() const
{
    return d_str.data();
}

std::strong_ordering operator<=>(Date const& lhs, Date const& rhs) noexcept
{
    return lhs.d_date <=> rhs.d_date;
}

bool operator==(Date const& lhs, Date const& rhs) noexcept
{
    return lhs.d_date == rhs.d_date;
}

chrono::year_month_day _strToYMD(string const& ymdStr)
{
    char delimiter1, delimiter2;
    int d, m, y;

    if(istringstream iss(ymdStr);
       ymdStr.size() == 10 && iss >> d >> delimiter1 >> m >> delimiter2 >> y)
        if(delimiter1 == delimiter2 && delimiter2 == '/')
            if(y >= 0)
            {
                auto const ret =
                    chrono::year_month_day(chrono::year(y), chrono::month(m), chrono::day(d));
                if(ret.ok())
                    return ret;
            }

    throw invalid_argument("Bad date string: "s + ymdStr);
}

array<char, 11> _ymdToStr(chrono::year_month_day ymd)
{
    array<char, 11> ret;

    unsigned const dd = static_cast<unsigned>(ymd.day());
    unsigned const mm = static_cast<unsigned>(ymd.month());
    unsigned const yyyy = static_cast<int>(ymd.year());

    // Pad leading zeros if needed
    sprintf(ret.data(), "%02d/%02d/%04d", dd, mm, yyyy);

    return ret;
}