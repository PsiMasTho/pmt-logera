#pragma once

#include "logera/fw_decl/fw_decl.hpp"
#include "logera/i_task_executor/i_task_executor.hpp"

#include <memory>

NS_FW_DECL_CLASS(pmt, i_task)

namespace pmt
{

class serial_task_executor : public i_task_executor
{
public:
  void execute(std::shared_ptr<i_task> task_) override;
  void wait() override;
};

} // namespace pmt