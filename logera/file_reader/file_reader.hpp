#pragma once

#include "logera/fw_decl/fw_decl.hpp"
#include "logera/i_file_reader/i_file_reader.hpp"
#include "logera/path_view/path_view.hpp"

#include <string>
#include <string_view>

NS_FW_DECL_CLASS(pmt, i_warning_collector)

namespace pmt
{

class file_reader : public i_file_reader
{
public:
  explicit file_reader(i_warning_collector& warning_collector_);
  auto read(path_view path_) -> std::string_view override;

private:
  i_warning_collector& _warning_collector;
  std::string          _buffer;
};

} // namespace pmt