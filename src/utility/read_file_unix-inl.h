// clang-format off
#ifdef __INTELLISENSE__
    #pragma once
    #include "utility.cc"
#endif
// clang-format on

#include <fcntl.h>
#include <unistd.h>

auto read_file_unix(char const* filename) -> buffer_t
{
    // open file and make an RAII guard
    int fd = open(filename, O_RDONLY);
    static auto const fd_closer = [](int* fd) { close(*fd); };
    unique_ptr<int, decltype(fd_closer)> const fd_guard(&fd, fd_closer);

    if(fd == -1)
        return make_pair(nullptr, 0);

    // find size of file
    u32 const size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    // Advise the kernel of our access pattern.
    posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);

    // create buffer
    auto buffer = make_unique_for_overwrite<char[]>(size + 1);
    buffer[size] = '\0';

    // read file
    auto bytes_read = read(fd, buffer.get(), size);
    if(bytes_read == -1 || bytes_read != size)
        return make_pair(nullptr, 0);

    return make_pair(std::move(buffer), size);
}