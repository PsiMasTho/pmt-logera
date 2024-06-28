#include "test.hpp"

#include "logera/file_reader/file_reader.hpp"

#include "logera/path_view/path_view.hpp"
#include "logera/serial_warning_collector/serial_warning_collector.hpp"
#include "logera/string_cast/string_cast.hpp"

#include <chrono>
#include <iostream>
#include <vector>

auto main(int argc, char** argv) -> int
{
  if (argc != 2)
  {
    std::cerr << "Usage: test_file_reader <file>\n";
    return 1;
  }

  pmt::serial_warning_collector warning_collector;

  pmt::file_reader reader{ warning_collector };

  auto const             start  = std::chrono::high_resolution_clock::now();
  std::string_view const result = reader.read(pmt::path_view{ argv[1] });
  auto const             end    = std::chrono::high_resolution_clock::now();

  std::cout << "Read " << result.size() << " bytes\n";
  std::cout << "Elapsed time: " << pmt::string_cast(std::chrono::duration_cast<std::chrono::microseconds>(end - start))
            << '\n';

  std::vector<pmt::warning> warnings;
  warning_collector.release_warnings(std::back_inserter(warnings));

  for (const auto& warning : warnings)
    std::cout << "Warning: " << warning._msg << '\n';
}