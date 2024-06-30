#pragma once

#include "logera/fw_decl/fw_decl.hpp"
#include "logera/i_task/i_task.hpp"
#include "logera/multifile/multifile.hpp"

#include <filesystem>
#include <optional>
#include <variant>

NS_FW_DECL_CLASS(pmt, i_file_reader)
NS_FW_DECL_CLASS(pmt, i_warning_collector)

namespace pmt
{

class file_parse_task : public i_task
{
public:
  file_parse_task(
    i_file_reader&        file_reader_,
    i_warning_collector&  warning_collector_,
    std::filesystem::path filepath_);

  auto get_result() -> std::optional<std::variant<log_file, header_file>>&;

private:
  void execute_impl() override;

  i_file_reader&                                     _file_reader;
  i_warning_collector&                               _warning_collector;
  std::filesystem::path                              _filepath;
  std::optional<std::variant<log_file, header_file>> _result;
};

} // namespace pmt