#include "atomic_warning_collector.hpp"

#include <cstddef>

namespace pmt
{

void atomic_warning_collector::add_warning(int code_, warning::location location_, std::string msg_)
{
  _warnings.acquire()->emplace_back(warning{ std::move(location_), std::move(msg_), code_ });
}

void atomic_warning_collector::clear_warnings()
{
  _warnings.acquire()->clear();
}

auto atomic_warning_collector::warning_count() -> size_t
{
  return _warnings.acquire()->size();
}

void atomic_warning_collector::release_warnings_impl(std::function<void(warning)> collector_)
{
  auto& warnings = *_warnings.acquire();
  for (auto& warning : warnings)
    collector_(std::move(warning));
  warnings.clear();
}

} // namespace pmt