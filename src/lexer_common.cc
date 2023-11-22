#include "lexer_common.h"

    // C++
#include <stdexcept> // runtime_error

    // C
#include <cstring>   // memchr

    // POSIX
#include <unistd.h>  // read, close
#include <fcntl.h>   // open, O_RDONLY, posix_fadvise

using namespace std;

namespace
{

auto read_file(char const* filename, int* size) -> unique_ptr<char[]>
{
        // open file and make an RAII guard
    int fd = open(filename, O_RDONLY);
    static auto const fd_closer = [](int* fd) { close(*fd); };
    unique_ptr<int, decltype(fd_closer)> const fd_guard(&fd, fd_closer);

    if (fd == -1)
        return nullptr;

        // find size of file
    *size = lseek(fd, 0, SEEK_END) + 1;
    lseek(fd, 0, SEEK_SET);

        // Advise the kernel of our access pattern.
    posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);

        // create buffer
    auto buffer = make_unique_for_overwrite<char[]>(*size + 1);
    buffer[*size] = '\0';

        // read file
    auto bytes_read = read(fd, buffer.get(), *size);
    if (bytes_read == -1)
        return nullptr;

    return buffer;
}

} // namespace

lexer_base::lexer_base(char const* infile)
:   m_filename(infile)
,   m_buffer(nullptr)
,   m_size(0)
{
    m_buffer = read_file(infile, &m_size);

    if (!m_buffer)
        throw runtime_error("Could not open file: " + string{infile});
}

auto lexer_base::line_nr(int ch) const -> int
{
    int ret = 1;
    char const* p = m_buffer.get();
    while (p)
    {
        p = (char*) memchr(p, '\n', ch - (p - m_buffer.get()));
        int const incr = p ? 1 : 0;
        ret += incr;
        p += incr;
    }
    return ret;
}

auto lexer_base::filename() const -> std::string const&
{
    return m_filename;
}
