#pragma once

#include <concepts>
#include <memory>

namespace pmt
{

template <std::default_initializable T>
class singleton
{
public:
  using value_type    = T;
  using shared_handle = std::shared_ptr<value_type>;
  static auto instance() -> shared_handle;
  ~singleton() = default;

private:
  singleton()                                    = default;
  singleton(singleton const&)                    = delete;
  auto operator=(singleton const&) -> singleton& = delete;
};

} // namespace pmt

#include "singleton-inl.hpp"