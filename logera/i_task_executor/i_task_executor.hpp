#pragma once

#include "logera/fw_decl/fw_decl.hpp"

#include <memory>

NS_FW_DECL_CLASS(pmt, i_task)

namespace pmt
{

class i_task_executor
{
public:
  virtual ~i_task_executor() = default;

  virtual void execute(std::shared_ptr<i_task> task_) = 0;
  virtual void wait()                                 = 0;
};

} // namespace pmt