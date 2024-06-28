// clang-format off
#ifdef __INTELLISENSE__
    #include "i_warning_collector.hpp"
#endif
// clang-format on

namespace pmt
{

template <std::output_iterator<warning> OUTPUT_IT>
void i_warning_collector::release_warnings(OUTPUT_IT dest_)
{
  // NOLINTNEXTLINE(performance-unnecessary-value-param)
  release_warnings_impl([&dest_](warning warning_) { *dest_++ = std::move(warning_); });
}

} // namespace pmt