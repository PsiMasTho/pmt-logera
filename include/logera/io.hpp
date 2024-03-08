#pragma once

#include "errors.hpp"

#include <string>
#include <string_view>

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
auto readallf(char const* path, std::string& buffer, error::container& errors) -> bool;

} // namespace io

namespace error
{

struct cannot_open_file : with_filename, with_unformatted_msg
{
public:
    cannot_open_file(std::string_view const path)
        : with_filename(path)
        , with_unformatted_msg("cannot open file")
    {
    }
};

struct failed_reading_file : with_filename, with_unformatted_msg
{
    failed_reading_file(std::string_view const path)
        : with_filename(path)
        , with_unformatted_msg("failed reading file")
    {
    }
};

} // namespace error