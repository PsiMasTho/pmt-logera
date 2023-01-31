#include "../src/date/date.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <type_traits>

using namespace std;
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(date_tests)

static_assert(is_nothrow_move_constructible<Date>::value,
              "Date should be noexcept MoveConstructible");

BOOST_AUTO_TEST_CASE(nonthrow_construction)
{
    Date a("1000-01-02");
    Date b("1992-12-23");
    Date c("2024-08-02");
    [[maybe_unused]] Date d = b;
}

BOOST_AUTO_TEST_CASE(throw_construction)
{
    auto const f = [](string const& str) { return Date(str); };

    BOOST_REQUIRE_THROW(f(""), invalid_argument);
    BOOST_REQUIRE_THROW(f("12345678x"), invalid_argument);
    BOOST_REQUIRE_THROW(f("12-34-5678"), invalid_argument);
    BOOST_REQUIRE_THROW(f("2022-00-06"), invalid_argument);
    BOOST_REQUIRE_THROW(f("0000-00-00"), invalid_argument);
    BOOST_REQUIRE_THROW(f("20225-01-12"), invalid_argument);
    BOOST_REQUIRE_THROW(f("01x02x2022"), invalid_argument);
    BOOST_REQUIRE_THROW(f("-2022-1-2"), invalid_argument);
    BOOST_REQUIRE_THROW(f("2022-01--2"), invalid_argument);
    BOOST_REQUIRE_THROW(f("-222-01-02"), invalid_argument);
    BOOST_REQUIRE_THROW(f("2001/12/03"), invalid_argument);
}

BOOST_AUTO_TEST_CASE(equality_comparisons)
{
    Date a("1998-02-02");
    Date b("1999-02-02");
    Date c("2022-04-05");
    Date d("1000-01-02");
    Date e = a;
    BOOST_TEST((a < b));
    BOOST_TEST((b > a));
    BOOST_TEST((d < a));
    BOOST_TEST((b != d));
    BOOST_TEST((a == e));
}

BOOST_AUTO_TEST_CASE(to_string_conversion)
{
    string const aStr = "1998-09-01";
    string const bStr = "2045-04-25";
    string const cStr = "1111-12-30";
    string const dStr = "2101-09-11";

    Date const aDate(aStr);
    Date const bDate(bStr);
    Date const cDate(cStr);
    Date const dDate(dStr);

    BOOST_TEST(aDate.to_string() == aStr);
    BOOST_TEST(bDate.to_string() == bStr);
    BOOST_TEST(cDate.to_string() == cStr);
    BOOST_TEST(dDate.to_string() == dStr);

    Date const eDate = dDate;
    BOOST_TEST(eDate.to_string() == dStr);
}

BOOST_AUTO_TEST_SUITE_END()