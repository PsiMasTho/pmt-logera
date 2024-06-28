#pragma once

#include <mutex>

namespace pmt
{

template <typename T>
class atomic_object
{
public:
  class unique_handle;
  using value_type = T;

  template <typename... ARGS>
  atomic_object(ARGS&&... args_);

  auto acquire() -> unique_handle;

private:
  T          _value;
  std::mutex _mutex;
};

template <typename T>
class atomic_object<T>::unique_handle
{
  friend atomic_object;

public:
  using value_type = T;

  auto operator*() -> T&;
  auto operator->() -> T*;
  auto get() -> T*;

private:
  unique_handle(T& value_, std::mutex& mutex_);

  T&                           _value;
  std::scoped_lock<std::mutex> _lock;
};

} // namespace pmt

#include "atomic_object-inl.hpp"