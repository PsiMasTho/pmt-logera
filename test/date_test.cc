#include <catch2/catch_test_macros.hpp>

#include "../src/date.h"

#include <string>
#include <type_traits>

using namespace std;

static_assert(is_nothrow_move_constructible<date>::value,
              "date should be noexcept move_constructible");

TEST_CASE("nonthrow_construction")
{
    date a("1000-01-02");
    date b("1992-12-23");
    date c("2024-08-02");
    [[maybe_unused]] date d = b;
}

TEST_CASE("throw_construction")
{
    auto const f = [](string const& str) { return date(str); };

    REQUIRE_THROWS_AS(f(""), invalid_argument);
    REQUIRE_THROWS_AS(f("12345678x"), invalid_argument);
    REQUIRE_THROWS_AS(f("12-34-5678"), invalid_argument);
    REQUIRE_THROWS_AS(f("2022-00-06"), invalid_argument);
    REQUIRE_THROWS_AS(f("0000-00-00"), invalid_argument);
    REQUIRE_THROWS_AS(f("20225-01-12"), invalid_argument);
    REQUIRE_THROWS_AS(f("20225-1-12"), invalid_argument);
    REQUIRE_THROWS_AS(f("01x02x2022"), invalid_argument);
    REQUIRE_THROWS_AS(f("-2022-1-2"), invalid_argument);
    REQUIRE_THROWS_AS(f("2022-01--2"), invalid_argument);
    REQUIRE_THROWS_AS(f("-222-01-02"), invalid_argument);
    REQUIRE_THROWS_AS(f("2001/12/03"), invalid_argument);
}

TEST_CASE("equality_comparisons")
{
    date a("1998-02-02");
    date b("1999-02-02");
    date c("2022-04-05");
    date d("1000-01-02");
    date e = a;
    REQUIRE((a < b));
    REQUIRE((b > a));
    REQUIRE((d < a));
    REQUIRE((b != d));
    REQUIRE((a == e));
}

TEST_CASE("to_string_conversion")
{
    string const aStr = "1998-09-01";
    string const bStr = "2045-04-25";
    string const cStr = "1111-12-30";
    string const dStr = "2101-09-11";

    date const aDate(aStr);
    date const bDate(bStr);
    date const cDate(cStr);
    date const dDate(dStr);

    REQUIRE(aDate.to_string() == aStr);
    REQUIRE(bDate.to_string() == bStr);
    REQUIRE(cDate.to_string() == cStr);
    REQUIRE(dDate.to_string() == dStr);

    date const eDate = dDate;
    REQUIRE(eDate.to_string() == dStr);
}