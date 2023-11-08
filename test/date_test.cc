//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>

#include "../include/log_date.h"

#include <string>
#include <type_traits>

using namespace std;

static_assert(is_nothrow_move_constructible<log_date>::value, "log_date should be nothrow_move_constructible");

TEST_CASE("nonthrow_construction")
{
    log_date a("1000-01-02");
    log_date b("1992-12-23");
    log_date c("2024-08-02");
    [[maybe_unused]] log_date d = b;
}

TEST_CASE("throw_construction")
{
    auto const f = [](string const& str) { return log_date(str); };

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
    log_date a("1998-02-02");
    log_date b("1999-02-02");
    log_date c("2022-04-05");
    log_date d("1000-01-02");
    log_date e = a;
    REQUIRE((a < b));
    REQUIRE((b > a));
    REQUIRE((d < a));
    REQUIRE((b != d));
    REQUIRE((a == e));
}

TEST_CASE("to_string_conversion")
{
    string const a_str = "1998-09-01";
    string const b_str = "2045-04-25";
    string const c_str = "1111-12-30";
    string const d_str = "2101-09-11";

    log_date const a_date(a_str);
    log_date const b_date(b_str);
    log_date const c_date(c_str);
    log_date const d_date(d_str);

    REQUIRE(a_date.to_string() == a_str);
    REQUIRE(b_date.to_string() == b_str);
    REQUIRE(c_date.to_string() == c_str);
    REQUIRE(d_date.to_string() == d_str);

    log_date const e_date = d_date;
    REQUIRE(e_date.to_string() == d_str);
}