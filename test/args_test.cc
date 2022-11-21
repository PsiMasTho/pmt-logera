#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif

#define BOOST_TEST_MODULE args-test
#include <boost/test/unit_test.hpp>

#include "../src/args/args.h"

using namespace std;

namespace
{
	Args::Opt const options[]
    {
        Args::Opt(Args::Opt::ValType::NONE,     "alpha",      'A'),
        Args::Opt(Args::Opt::ValType::NONE,     "beta",       'b'),
        Args::Opt(Args::Opt::ValType::NONE,     "CHARLIE",    'C'),
        Args::Opt(Args::Opt::ValType::NONE,     "ECHO",       'e'),
        Args::Opt(Args::Opt::ValType::NONE,     "FoX-TrOt",   'f'),

        Args::Opt(Args::Opt::ValType::OPTIONAL, "golf",       'G'),
        Args::Opt(Args::Opt::ValType::OPTIONAL, "hotel",      'h'),
        Args::Opt(Args::Opt::ValType::OPTIONAL, "INDIA",      'I'),
        Args::Opt(Args::Opt::ValType::OPTIONAL, "JULIETT",    'j'),
        Args::Opt(Args::Opt::ValType::OPTIONAL, "Ki-Lo",      'k'),

        Args::Opt(Args::Opt::ValType::REQUIRED, "lima",       'L'),
        Args::Opt(Args::Opt::ValType::REQUIRED, "mike",       'm'),
        Args::Opt(Args::Opt::ValType::REQUIRED, "NOVEMBER",   'N'),
        Args::Opt(Args::Opt::ValType::REQUIRED, "OSCAR",      'o'),
        Args::Opt(Args::Opt::ValType::REQUIRED, "Pa-Pa",      'P'),
    };
    size_t const nOptions = size(options);
}

BOOST_AUTO_TEST_CASE( constructors_test_nothrow )
{
	char const* const argv[]
	{
		"./a.out",
		"-A",
		"-G",      "gVal",
		"--beta",
		"--Pa-Pa", "foo", "bar", "baz",
		"--hotel",
		"--INDIA", "12345.222",
		"-k", 	   "3.14169",
		"--OSCAR", "__SoMeThing__ ;)",
		nullptr
	};

	BOOST_REQUIRE_NO_THROW(Args(options, nOptions, argv));
}

BOOST_AUTO_TEST_CASE( constructors_test_throw )
{
	char const* const argv[]
	{
		"./a.out",
		"--unknown-option-that-causes-a-throw", "foo",
		nullptr
	};
	
	BOOST_REQUIRE_THROW(Args(options, nOptions, argv), invalid_argument);
}

// todo: test Option equality operators

BOOST_AUTO_TEST_CASE( value_check )
{
	char const* const argv[]
	{
		"./a.out",
		"--golf",  "firstG",  "123",
		"-G",      "secondG", "456",
        "--Ki-Lo", "firstK",  "789",
        "--lima",  "firstL",  "101",
		"-L",      "secondL", "112",
		"-o",	   "firstO",  "131",
		"--alpha",
		"-b",
		nullptr
	};

	Args args(options, nOptions, argv);

	BOOST_TEST(args.argv0().second == "./a.out");
	
	{
		auto const [wasPresent, res] = args.option("ECHO");
		BOOST_TEST( (!wasPresent && res.empty()) );
	}

	{
		auto const [wasPresentShort, resShort] = args.option('G');
		auto const [wasPresentLong, resLong] = args.option("golf");
		BOOST_TEST( (wasPresentShort && wasPresentLong) );
		BOOST_TEST( (resShort == "firstG 123") );
		BOOST_TEST( (!resShort.empty() == !resLong.empty()) );
	}

	{
		auto const [wasPresent, res] = args.option("Ki-Lo");
		BOOST_TEST( (wasPresent && res == "firstK 789") );
	}

	{
		BOOST_REQUIRE_THROW( args.option("g"), invalid_argument );
	}

	{
		auto const [wasPresent, res] = args.option("alpha");
		BOOST_TEST( (wasPresent && res.empty()) );
	}
}