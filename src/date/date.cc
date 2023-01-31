#include "date.h"

#include <cstdio>
#include <exception>
#include <sstream>
#include <utility>

using namespace std;

namespace
{

enum
{
    DATE_REQ_LEN = 10
};

chrono::year_month_day strToYMD(string const& ymdStr)
{
    char delimiter1;
    char delimiter2;
    unsigned d; // day
    unsigned m; // month
    int y; // year

    if(istringstream iss(ymdStr);
       ymdStr.size() == DATE_REQ_LEN && iss >> y >> delimiter1 >> m >> delimiter2 >> d)
        if(delimiter1 == delimiter2 && delimiter2 == '-')
            if(y >= 0)
            {
                auto const ret =
                    chrono::year_month_day(chrono::year(y), chrono::month(m), chrono::day(d));
                if(ret.ok())
                    return ret;
            }

    throw invalid_argument("Bad date string: "s + ymdStr);
}

array<char, DATE_REQ_LEN + 1> ymdToStr(chrono::year_month_day ymd)
{
    array<char, DATE_REQ_LEN + 1> ret;
    ret.fill(0);

    auto const dd = static_cast<unsigned>(ymd.day());
    auto const mm = static_cast<unsigned>(ymd.month());
    auto const yyyy = static_cast<int>(ymd.year());

    // Pad leading zeros if needed
    if(snprintf(ret.data(), ret.size(), "%04d-%02u-%02u", yyyy, mm, dd) != DATE_REQ_LEN)
        throw invalid_argument("Bad date: day: "s + to_string(dd) + " month: " + to_string(mm) +
                               " year: " + to_string(yyyy));

    return ret;
}

} // namespace

Date::Date(string const& dateStr)
    : d_date(strToYMD(dateStr))
    , d_str(ymdToStr(d_date))
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