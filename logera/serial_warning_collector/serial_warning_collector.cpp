#include "serial_warning_collector.hpp"

#include <cstddef>

namespace pmt
{

void serial_warning_collector::add_warning(int code_, warning::location location_, std::string msg_)
{
  _warnings.emplace_back(warning{ std::move(location_), std::move(msg_), code_ });
}

void serial_warning_collector::clear_warnings()
{
  _warnings.clear();
}

auto serial_warning_collector::warning_count() -> size_t
{
  return _warnings.size();
}

void serial_warning_collector::release_warnings_impl(std::function<void(warning)> collector_)
{
  for (auto& warning : _warnings)
    collector_(std::move(warning));

  _warnings.clear();
}

} // namespace pmt