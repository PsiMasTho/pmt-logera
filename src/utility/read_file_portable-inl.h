// clang-format off
#ifdef __INTELLISENSE__
    #pragma once
    #include "utility.cc"
#endif
// clang-format on

#include <cstdio>

auto read_file_portable(char const* filename) -> unique_ptr<char[]>
{
    // open file and make an RAII guard
    FILE* file = fopen(filename, "r");
    static auto const file_closer = [](FILE** file) { fclose(*file); };
    unique_ptr<FILE*, decltype(file_closer)> const file_guard(&file, file_closer);

    if (file == nullptr)
        return nullptr;

    // find size of file
    fseek(file, 0, SEEK_END);
    u32 const size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // create buffer
    auto buffer = make_unique_for_overwrite<char[]>(size + 1);
    buffer[size] = '\0';

    // read file
    auto bytes_read = fread(buffer.get(), 1, size, file);
    if (bytes_read != size)
        return nullptr;

    return buffer;
}