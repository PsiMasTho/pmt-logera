#include "serial_task_executor.hpp"

#include "logera/i_task/i_task.hpp"

namespace pmt
{

void serial_task_executor::execute(std::shared_ptr<i_task> task_)
{
  task_->execute();
}

void serial_task_executor::wait()
{
}

} // namespace pmt