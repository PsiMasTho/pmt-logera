#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <vector>

namespace pmt
{

template <typename T>
class object_pool;

template <typename T>
void swap(typename object_pool<T>::unique_handle& lhs_, typename object_pool<T>::unique_handle& rhs_);

template <typename T>
class object_pool
{
public:
  class unique_handle;
  using value_type = T;

  object_pool(std::size_t size_);
  auto acquire() -> unique_handle;
  auto get(std::size_t index_) -> value_type&;
  auto size() const -> std::size_t;

private:
  void release(std::size_t index_);

  struct alignas(64) aligned_value_type : value_type
  {
  };

  std::vector<aligned_value_type> _pool;
  std::vector<bool>               _in_use;
  std::mutex                      _mutex;
  std::condition_variable         _cv;
};

template <typename T>
class object_pool<T>::unique_handle
{
public:
  using value_type = T;

private:
  friend object_pool;
  friend void swap<T>(unique_handle& lhs_, unique_handle& rhs_);

  value_type&                _item;
  object_pool*               _pool;
  std::optional<std::size_t> _index;

public:
  unique_handle(unique_handle&&) noexcept;
  unique_handle(unique_handle const&) = delete;
  auto operator=(unique_handle&&) noexcept -> unique_handle&;
  auto operator=(unique_handle const&) -> unique_handle& = delete;
  ~unique_handle();

  auto operator*() -> value_type&;
  auto operator->() -> value_type*;
  auto get() -> value_type*;

private:
  unique_handle(value_type& item_, object_pool* pool_, std::size_t index_);
};

} // namespace pmt

#include "object_pool-inl.hpp"