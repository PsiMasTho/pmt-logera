// clang-format off
#ifdef __INTELLISENSE__
    #include "object_pool.hpp"
#endif
// clang-format on

#include <cassert>
#include <utility> // exchange, swap

namespace pmt
{

template <typename T>
void swap(typename object_pool<T>::unique_handle& lhs_, typename object_pool<T>::unique_handle& rhs_)
{
  std::swap(lhs_.item_, rhs_.item_);
  std::swap(lhs_._pool, rhs_._pool);
  std::swap(lhs_.index_, rhs_.index_);
}

template <typename T>
object_pool<T>::object_pool(std::size_t size_)
  : _pool(size_)
  , _in_use(size_, false)
  , _mutex()
  , _cv()
{
}

template <typename T>
auto object_pool<T>::acquire() -> unique_handle
{
  std::unique_lock lock{ _mutex };

  while (true)
  {
    for (std::size_t i = 0; i < _in_use.size(); ++i)
    {
      if (!_in_use[i])
      {
        _in_use[i] = true;
        return { _pool[i], this, i };
      }
    }

    _cv.wait(lock);
  }
}

template <typename T>
auto object_pool<T>::get(std::size_t index_) -> value_type&
{
  return _pool[index_];
}

template <typename T>
auto object_pool<T>::size() const -> std::size_t
{
  return _pool.size();
}

template <typename T>
void object_pool<T>::release(std::size_t index_)
{
  std::unique_lock lock{ _mutex };

  assert(_in_use[index_]);

  _in_use[index_] = false;
  _cv.notify_one();
}

template <typename T>
object_pool<T>::unique_handle::unique_handle(unique_handle&& other_) noexcept
  : _item{ other_._item }
  , _pool{ other_._pool }
  , _index{ std::exchange(other_._index, std::nullopt) }
{
}

template <typename T>
auto object_pool<T>::unique_handle::operator=(unique_handle&& other_) noexcept -> unique_handle&
{
  if (this == &other_)
    return *this;

  swap(*this, other_);
  return *this;
}

template <typename T>
object_pool<T>::unique_handle::~unique_handle()
{
  if (_index.has_value())
    _pool->release(*_index);
}

template <typename T>
auto object_pool<T>::unique_handle::operator*() -> value_type&
{
  return _item;
}

template <typename T>
auto object_pool<T>::unique_handle::operator->() -> value_type*
{
  return &_item;
}

template <typename T>
auto object_pool<T>::unique_handle::get() -> value_type*
{
  return &_item;
}

template <typename T>
object_pool<T>::unique_handle::unique_handle(value_type& item_, object_pool* pool_, std::size_t index_)
  : _item{ item_ }
  , _pool{ pool_ }
  , _index{ index_ }
{
}

} // namespace pmt