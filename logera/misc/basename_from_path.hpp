#pragma once

#include "logera/path_view/path_view.hpp"

namespace pmt
{

/**
 * \brief Extracts the basename from a given path.
 *
 * @param path The input path.
 * @return The basename of the path.
 */
[[nodiscard]] auto basename_from_path(path_view path_) -> path_view;

} // namespace pmt