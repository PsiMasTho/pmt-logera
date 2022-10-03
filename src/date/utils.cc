#include "date.ih"

namespace
{
    constexpr array<pair<array<char, 4>, chrono::month>, 12> months =
    {
        pair(to_array("Jan"), chrono::January),
        pair(to_array("Feb"), chrono::February),
        pair(to_array("Mar"), chrono::March),
        pair(to_array("Apr"), chrono::April),
        pair(to_array("May"), chrono::May),
        pair(to_array("Jun"), chrono::June),
        pair(to_array("Jul"), chrono::July),
        pair(to_array("Aug"), chrono::August),
        pair(to_array("Sep"), chrono::September),
        pair(to_array("Oct"), chrono::October),
        pair(to_array("Nov"), chrono::November),
        pair(to_array("Dec"), chrono::December)
    };

    constexpr array<char, 128> _tolowerLookup =
    {
          0,
          1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
         33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45 ,46 ,47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', 91, 92, 93, 94, 95, 96,
         97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127
    };
}

constexpr chrono::month _strToM(string const &monthStr)
{
    return find_if(begin(months), end(months), [&monthStr](auto const &val)
                   { 
                        return _icaseStrCmp(val.first.data(), monthStr.c_str());
                   })->second;
}

chrono::year_month_day _strToYMD(string const &ymdStr)
{
    chrono::year const yr(stoull(ymdStr.substr(5, 4)));
    chrono::month const mth(_strToM(ymdStr.substr(2, 3)));
    chrono::day const dy(stoull(ymdStr.substr(0, 2)));

    return chrono::year_month_day(yr, mth, dy);
}

string _ymdToStr(chrono::year_month_day ymd)
{
    string const dy = to_string(unsigned(ymd.day()));
    string const mth(months[unsigned(ymd.month()) - 1].first.data());
    string const yr = to_string(int(ymd.year()));

    return dy + mth + yr;
}

constexpr char _toLower(char ch)
{
    return _tolowerLookup[ch];
}

constexpr bool _icaseStrCmp(char const* a, char const* b)
{
    return _toLower(*a) == _toLower(*b) && (*a == '\0' || _icaseStrCmp(a + 1, b + 1));
}