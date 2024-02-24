#pragma once

#include "errors.hpp"

#include <string>
#include <vector>

namespace io
{

/**
 * @brief Extracts the basename from a given path.
 *
 * @param path The input path.
 * @return The basename of the path.
 */
auto basename_from_path(std::string_view const path) -> std::string_view;

/**
 * @brief Reads the entire file into a buffer.
 *
 * @param path The path to the file.
 * @param buffer The buffer to store the file contents in. Will replace the
 * existing contents.
 * @param errors Any errors encountered during reading.
 * @return true on success, false on failure.
 */
auto readallf(char const* path, std::string& buffer, std::vector<error::record>& errors) -> bool;

} // namespace io
