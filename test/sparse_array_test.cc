//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>

#include "../include/detail/sparse_array.h"

#include <string>
#include <type_traits>

using namespace std;

static_assert(is_nothrow_move_constructible<sparse_array<string>>::value,
              "sparse_array should be nothrow_move_constructible");

TEST_CASE("capacity")
{
    sparse_array<int> arr1(20);
    REQUIRE(arr1.capacity() == 20);

    sparse_array<string> arr2(0);
    REQUIRE(arr2.capacity() == 0);
}

TEST_CASE("with_trivial_type")
{
    sparse_array<int> arr1(45);
    REQUIRE(arr1.exists(44) == false);

    arr1.set(44, 10000);
    REQUIRE(arr1.exists(44) == true);
    REQUIRE(arr1.get(44) == 10000);

    arr1.set(1, 1);
    arr1.set(2, 2);
    arr1.set(3, 3);
    arr1.set(4, 4);

    REQUIRE(arr1.get(44) == 10000);

    REQUIRE(arr1.get(1) == 1);
    REQUIRE(arr1.get(2) == 2);
    REQUIRE(arr1.get(3) == 3);
    REQUIRE(arr1.get(4) == 4);
}

TEST_CASE("with_string_type")
{
    sparse_array<string> arr1(45);
    REQUIRE(arr1.exists(44) == false);

    arr1.set(44, "10,000");
    REQUIRE(arr1.exists(44) == true);
    REQUIRE(arr1.get(44) == "10,000");

    arr1.set(1, "one");
    arr1.set(2, "two");
    arr1.set(3, "three"s);
    string four = "four";
    arr1.set(4, move(four));

    REQUIRE(arr1.get(44) == "10,000");

    REQUIRE(arr1.get(1) == "one");
    REQUIRE(arr1.get(2) == "two");
    REQUIRE(arr1.get(3) == "three");
    REQUIRE(arr1.get(4) == "four");
}