#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef struct opaque_vector opaque_vector;

/**
 * @brief Extracts the basename from a given path.
 *
 * @param path The input path.
 * @param dest The output buffer to store the basename.
 * @param bufsz The size of the output buffer, must be >= 1.
 */
void basename_from_path(char const* path, char* dest, int bufsz);

/**
 * @brief Reads the entire file into a buffer.
 *
 * @param path The path to the file.
 * @param buffer The buffer to store the file contents, resized as needed and null-terminated.
 * @param errors Any errors encountered during reading.
 * @return true if the file was read successfully, false otherwise.
 */
bool readallf(char const* path, opaque_vector* buffer, opaque_vector* errors);
