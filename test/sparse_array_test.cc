#include "../src/sparse_array/sparse_array.h"

#include <boost/test/unit_test.hpp>

#include <string>
#include <type_traits>

using namespace std;
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(sparse_array_tests)

static_assert(is_nothrow_move_constructible<SparseArray<string>>::value, "SparseArray should be noexcept MoveConstructible");

BOOST_AUTO_TEST_CASE(capacity)
{
    SparseArray<int> arr1(20);
	BOOST_TEST(arr1.capacity() == 20);

    SparseArray<string> arr2(0);
	BOOST_TEST(arr2.capacity() == 0);
}

BOOST_AUTO_TEST_CASE(with_trivial_type)
{
    SparseArray<int> arr1(45);
	BOOST_TEST(arr1.exists(44) == false);

	arr1.set(44, 10000);
	BOOST_TEST(arr1.exists(44) == true);
	BOOST_TEST(arr1.get(44) == 10000);

	arr1.set(1, 1);
	arr1.set(2, 2);
	arr1.set(3, 3);
	arr1.set(4, 4);

	BOOST_TEST(arr1.get(44) == 10000);

	BOOST_TEST(arr1.get(1) == 1);
	BOOST_TEST(arr1.get(2) == 2);
	BOOST_TEST(arr1.get(3) == 3);
	BOOST_TEST(arr1.get(4) == 4);
}

BOOST_AUTO_TEST_CASE(with_string_type)
{
    SparseArray<string> arr1(45);
	BOOST_TEST(arr1.exists(44) == false);

	arr1.set(44, "10,000");
	BOOST_TEST(arr1.exists(44) == true);
	BOOST_TEST(arr1.get(44) == "10,000");

	arr1.set(1, "one");
	arr1.set(2, "two");
	arr1.set(3, "three"s);
	string four = "four";
	arr1.set(4, move(four));

	BOOST_TEST(arr1.get(44) == "10,000");

	BOOST_TEST(arr1.get(1) == "one");
	BOOST_TEST(arr1.get(2) == "two");
	BOOST_TEST(arr1.get(3) == "three");
	BOOST_TEST(arr1.get(4) == "four");
}

BOOST_AUTO_TEST_SUITE_END()