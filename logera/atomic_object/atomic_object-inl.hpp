// clang-format off
#ifdef __INTELLISENSE__
    #include "atomic_object.hpp"
#endif
// clang-format on

namespace pmt
{

template <typename T>
template <typename... ARGS>
atomic_object<T>::atomic_object(ARGS&&... args_)
  : _value(std::forward<ARGS>(args_)...)
{
}

template <typename T>
auto atomic_object<T>::acquire() -> unique_handle
{
  return { _value, _mutex };
}

template <typename T>
auto atomic_object<T>::unique_handle::operator*() -> T&
{
  return _value;
}

template <typename T>
auto atomic_object<T>::unique_handle::operator->() -> T*
{
  return &_value;
}

template <typename T>
auto atomic_object<T>::unique_handle::get() -> T*
{
  return &_value;
}

template <typename T>
atomic_object<T>::unique_handle::unique_handle(T& value_, std::mutex& mutex_)
  : _value(value_)
  , _lock(mutex_)
{
}

} // namespace pmt