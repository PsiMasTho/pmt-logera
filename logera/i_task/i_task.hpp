#pragma once

namespace pmt
{

class i_task
{
public:
  i_task();
  virtual ~i_task() = default;
  void execute();
  auto is_done() const -> bool;

private:
  virtual void execute_impl() = 0;

  bool _done = false;
};

} // namespace pmt