#include <boost/test/unit_test.hpp>

#include "../src/arg_parser/arg_parser.h"
#include "../src/args/args.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(args_tests)

namespace
{
Args::Opt const options[]{
    Args::Opt(Args::ValType::NONE, "alpha", 'A'),
    Args::Opt(Args::ValType::NONE, "beta", 'b'),
    Args::Opt(Args::ValType::NONE, "CHARLIE", 'C'),
    Args::Opt(Args::ValType::NONE, "ECHO", 'e'),
    Args::Opt(Args::ValType::NONE, "FoX-TrOt", 'f'),

    Args::Opt(Args::ValType::OPTIONAL, "golf", 'G'),
    Args::Opt(Args::ValType::OPTIONAL, "hotel", 'h'),
    Args::Opt(Args::ValType::OPTIONAL, "INDIA", 'I'),
    Args::Opt(Args::ValType::OPTIONAL, "JULIETT", 'j'),
    Args::Opt(Args::ValType::OPTIONAL, "Ki-Lo", 'k'),

    Args::Opt(Args::ValType::REQUIRED, "lima", 'L'),
    Args::Opt(Args::ValType::REQUIRED, "mike", 'm'),
    Args::Opt(Args::ValType::REQUIRED, "NOVEMBER", 'N'),
    Args::Opt(Args::ValType::REQUIRED, "OSCAR", 'o'),
    Args::Opt(Args::ValType::REQUIRED, "Pa-Pa", 'P'),
};
size_t const nOptions = std::size(options);
} // namespace

BOOST_AUTO_TEST_CASE(constructors_test_nothrow)
{
    char const* const argv[]{"./a.out",
                             "-A",
                             "-G",
                             "gVal",
                             "--beta",
                             "--Pa-Pa",
                             "foo",
                             "bar",
                             "baz",
                             "--hotel",
                             "--INDIA",
                             "12345.222",
                             "-k",
                             "3.14169",
                             "--OSCAR",
                             "__SoMeThing__ ;)",
                             nullptr};

    BOOST_REQUIRE_NO_THROW(Args(options, nOptions, DefaultArgParser{argv}));
}

BOOST_AUTO_TEST_CASE(constructors_test_throw_because_unknown_opt)
{
    char const* const argv[]{"./a.out", "--unknown-option-that-causes-a-throw", "foo", nullptr};

    BOOST_REQUIRE_THROW(Args(options, nOptions, DefaultArgParser{argv}), invalid_argument);
}

BOOST_AUTO_TEST_CASE(constructors_test_throw_because_duplicated_opt)
{
    char const* const argv[]{"./a.out", "-G", "garry", "--golf", "jerry", nullptr};

    BOOST_REQUIRE_THROW(Args(options, nOptions, DefaultArgParser{argv}), invalid_argument);
}

// todo: test Option equality operators

BOOST_AUTO_TEST_CASE(value_check)
{
    char const* const argv[]{"./a.out",
                             "--golf",
                             "firstG",
                             "123",
                             "--Ki-Lo",
                             "firstK",
                             "789",
                             "--lima",
                             "firstL",
                             "101",
                             "-o",
                             "firstO",
                             "131",
                             "--alpha",
                             "-b",
                             nullptr};

    Args args(options, nOptions, DefaultArgParser{argv});

    BOOST_TEST(args.argv0() == "./a.out");

    {
        auto const [wasPresent, res] = args.option("ECHO");
        BOOST_TEST((!wasPresent && res.empty()));
    }

    {
        auto const [wasPresentShort, resShort] = args.option('G');
        auto const [wasPresentLong, resLong] = args.option("golf");
        BOOST_TEST((wasPresentShort && wasPresentLong));
        BOOST_TEST(resShort == "firstG 123");
        BOOST_TEST((!resShort.empty() == !resLong.empty()));
    }

    {
        auto const [wasPresent, res] = args.option("Ki-Lo");
        BOOST_TEST((wasPresent && res == "firstK 789"));
    }

    {
        BOOST_REQUIRE_THROW(args.option("g"), invalid_argument);
    }

    {
        auto const [wasPresent, res] = args.option("alpha");
        BOOST_TEST((wasPresent && res.empty()));
    }
}

BOOST_AUTO_TEST_SUITE_END()