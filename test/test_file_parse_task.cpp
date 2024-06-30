#include "test.hpp"

#include "logera/file_parse_task/file_parse_task.hpp"

#include "logera/file_reader/file_reader.hpp"
#include "logera/serial_warning_collector/serial_warning_collector.hpp"
#include "logera/string_cast/string_cast.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <variant>
#include <vector>

auto main(int argc, char** argv) -> int
{
  if (argc < 2)
  {
    std::cerr << "Usage: test_file_parse_task <input_file>...\n";
    return 1;
  }

  pmt::serial_warning_collector warning_collector;
  pmt::file_reader              file_reader{ warning_collector };

  auto const start = std::chrono::high_resolution_clock::now();
  for (int i = 1; i < argc; ++i)
  {
    pmt::file_parse_task task{ file_reader, warning_collector, std::filesystem::path{ argv[i] } };
    task.execute();
  }
  auto const end = std::chrono::high_resolution_clock::now();

  std::cout << "Elapsed time: " << pmt::string_cast(std::chrono::duration_cast<std::chrono::microseconds>(end - start))
            << '\n';

  std::vector<pmt::warning> warnings;
  warning_collector.release_warnings(std::back_inserter(warnings));

  for (const auto& warning : warnings)
  {
    std::string const filename_component = warning._location._filepath.value_or("[n/a]").filename().string();
    std::string const lineno_component
      = warning._location._lineno ? std::to_string(*warning._location._lineno) : "[n/a]";
    std::string const colno_component = warning._location._colno ? std::to_string(*warning._location._colno) : "[n/a]";
    std::cout << "Warning: (" << filename_component << ':' << lineno_component << ':' << colno_component << ") "
              << warning._msg << '\n';
  }
}
