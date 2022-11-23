#include "../src/date/date.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <type_traits>

using namespace std;
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(date_tests)

static_assert(is_nothrow_move_constructible<Date>::value, "Date should be noexcept MoveConstructible");

BOOST_AUTO_TEST_CASE(nonthrow_construction)
{
    Date a("01/02/1000");
    Date b("23/12/1992");
    Date c("08/02/2024");
    [[maybe_unused]] Date d = b;
}

BOOST_AUTO_TEST_CASE(throw_construction)
{
    auto const f = [](string const& str) { return Date(str); };

    BOOST_REQUIRE_THROW(f(""), invalid_argument);
    BOOST_REQUIRE_THROW(f("12345678x"), invalid_argument);
    BOOST_REQUIRE_THROW(f("12/34/5678"), invalid_argument);
    BOOST_REQUIRE_THROW(f("00/06/2022"), invalid_argument);
    BOOST_REQUIRE_THROW(f("00/00/0000"), invalid_argument);
    BOOST_REQUIRE_THROW(f("01/12/20225"), invalid_argument);
    BOOST_REQUIRE_THROW(f("01x02x2022"), invalid_argument);
    BOOST_REQUIRE_THROW(f("-1/2/2022"), invalid_argument);
    BOOST_REQUIRE_THROW(f("01/-2/2022"), invalid_argument);
    BOOST_REQUIRE_THROW(f("01/02/-222"), invalid_argument);
}

BOOST_AUTO_TEST_CASE(equality_comparisons)
{
    Date a("02/02/1998");
    Date b("02/02/1999");
    Date c("04/05/2022");
    Date d("01/02/1000");
    Date e = a;
    BOOST_TEST((a < b));
    BOOST_TEST((b > a));
    BOOST_TEST((d < a));
    BOOST_TEST((b != d));
    BOOST_TEST((a == e));
}

BOOST_AUTO_TEST_CASE(to_string_conversion)
{
    string const aStr = "09/01/1998";
    string const bStr = "25/04/2045";
    string const cStr = "30/12/1111";
    string const dStr = "09/11/2101";

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