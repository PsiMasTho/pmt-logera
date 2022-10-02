#include "date.h"

#include <unordered_map>
#include <ostream>

using namespace std;

namespace
{
    unordered_map<string, chrono::month> const months
    {
        {"Jan", chrono::January},
        {"Feb", chrono::February},
        {"Mar", chrono::March},
        {"Apr", chrono::April},
        {"May", chrono::May},
        {"Jun", chrono::June},
        {"Jul", chrono::July},
        {"Aug", chrono::August},
        {"Sep", chrono::September},
        {"Oct", chrono::October},
        {"Nov", chrono::November},
        {"Dec", chrono::December}
    };
}

Date::Date(string const& dateStr)
:
    d_date{},
    d_string{dateStr}
{
    chrono::day dy(stoull(dateStr.substr(0, 2)));
    chrono::month mth(months.find(dateStr.substr(2,3))->second);
    chrono::year yr(stoull(dateStr.substr(5, 4)));

    d_date = chrono::year_month_day(yr, mth, dy);
}

string const& Date::to_string() const
{
    return d_string;
}

bool operator<(Date const& lhs, Date const& rhs)
{
    return lhs.d_date < rhs.d_date;
}