#include "test.hpp"

#if 0

#include "logera/file_parse_task/file_parse_task.hpp"

#include "logera/file_reader/file_reader.hpp"
#include "logera/serial_warning_collector/serial_warning_collector.hpp"
#include "logera/string_cast/string_cast.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>

auto main(int argc, char** argv) -> int
{
  if (argc != 2)
  {
    std::cerr << "Usage: test_file_parse_task <input_file>\n";
    return 1;
  }

  pmt::serial_warning_collector warning_collector;
  pmt::file_reader              file_reader{ warning_collector };

  pmt::file_parse_task task{ file_reader, warning_collector, std::filesystem::path{ argv[1] } };

  auto const start = std::chrono::high_resolution_clock::now();
  task.execute();
  auto const end = std::chrono::high_resolution_clock::now();
  assert(task.is_done());

  std::cout << "Elapsed time: " << pmt::string_cast(std::chrono::duration_cast<std::chrono::microseconds>(end - start))
            << '\n';

  std::optional<pmt::log_file> const result = std::move(task.get_result());

  std::vector<pmt::warning> warnings;
  warning_collector.release_warnings(std::back_inserter(warnings));

  for (const auto& warning : warnings)
    std::cout << "Warning: " << warning._msg << '\n';

  if (result)
    std::cout << "Number of lines: " << result->_lines.size() << '\n';
  else
    std::cout << "Task returned no result\n";
}

#endif

int main()
{
}