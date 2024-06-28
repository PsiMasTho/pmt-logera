#pragma once

#include "logera/i_warning_collector/i_warning_collector.hpp"
#include "logera/warning/warning.hpp"

#include <vector>

namespace pmt
{

class serial_warning_collector : public i_warning_collector
{
public:
  void add_warning(int code_, warning::location location_, std::string msg_) override;
  void clear_warnings() override;
  auto warning_count() -> std::size_t override;

private:
  void release_warnings_impl(std::function<void(warning)> collector_) override;

  std::vector<warning> _warnings;
};

} // namespace pmt
