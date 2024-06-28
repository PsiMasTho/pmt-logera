#include "i_task.hpp"

namespace pmt
{

i_task::i_task()
  : _done(false)
{
}

void i_task::execute()
{
  execute_impl();
  _done = true;
}

auto i_task::is_done() const -> bool
{
  return _done;
}

} // namespace pmt