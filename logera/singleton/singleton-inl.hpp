// clang-format off
#ifdef __INTELLISENSE__
    #include "singleton.hpp"
#endif
// clang-format on

#include <mutex>

namespace pmt
{

template <std::default_initializable T>
auto singleton<T>::instance() -> shared_handle
{
  static std::weak_ptr<T> instance;
  static std::mutex       mutex;

  std::scoped_lock l(mutex);

  if (!instance.expired())
    return instance.lock();

  shared_handle ret = std::make_shared<T>();
  instance          = ret;
  return ret;
}

} // namespace pmt