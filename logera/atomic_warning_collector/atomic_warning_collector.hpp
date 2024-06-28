#pragma once

#include "logera/atomic_object/atomic_object.hpp"
#include "logera/i_warning_collector/i_warning_collector.hpp"
#include "logera/warning/warning.hpp"

#include <vector>

namespace pmt
{

class atomic_warning_collector : public i_warning_collector
{
public:
  using container_type = std::vector<warning>;

  void add_warning(int code_, warning::location location_, std::string msg_) override;
  void clear_warnings() override;
  auto warning_count() -> std::size_t override;

private:
  void release_warnings_impl(std::function<void(warning)> collector_) override;

  atomic_object<container_type> _warnings;
};

} // namespace pmt
