#pragma once

#include "logera/warning/warning.hpp"

#include <functional>
#include <string>
#include <vector>

namespace pmt
{

class i_warning_collector
{
public:
  virtual ~i_warning_collector() = default;

  virtual void add_warning(int code_, warning::location location_, std::string msg_) = 0;
  virtual void clear_warnings()                                                      = 0;
  virtual auto warning_count() -> std::size_t                                        = 0;

  template <std::output_iterator<warning> OUTPUT_IT>
  void release_warnings(OUTPUT_IT dest_);

private:
  virtual void release_warnings_impl(std::function<void(warning)> collector_) = 0;
};

} // namespace pmt

#include "i_warning_collector-inl.hpp"