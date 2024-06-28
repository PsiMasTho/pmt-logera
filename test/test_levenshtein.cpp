#include "test.hpp"

#include "logera/levenshtein/levenshtein.hpp"

#include <iostream>
#include <string_view>

auto main(int argc, char* argv[]) -> int
{
  if (argc != 3)
  {
    std::cerr << "Usage: test_levenshtein <string1> <string2>\n";
    return 1;
  }

  std::string_view const str1{ argv[1] };
  std::string_view const str2{ argv[2] };

  std::cout << "Levenshtein distance between \"" << str1 << "\" and \"" << str2
            << "\": " << pmt::levenshtein().distance(str1, str2) << '\n';
}