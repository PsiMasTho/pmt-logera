#include "file_reader.hpp"

#include "logera/i_warning_collector/i_warning_collector.hpp"
#include "logera/warning_code/warning_code.hpp"

#include <utility>

using namespace std;

// clang-format off
#if defined(__unix__)
    extern "C"
    {
    #include <fcntl.h>
    #include <unistd.h>
    }

    namespace{
    class logera_fd {
        int _fd;
    public:
        logera_fd(int fd_) : _fd(fd_) {}
        logera_fd(logera_fd&& tmp_) noexcept : _fd{exchange(tmp_._fd, -1)} {}
        logera_fd(logera_fd const&) = delete;
        auto operator=(logera_fd const&) -> logera_fd& = delete;
        ~logera_fd() { if (_fd != -1) close(_fd); }
        
        auto get() const -> int { return _fd; }
    };

    using logera_open = decltype([](pmt::path_view path_) -> logera_fd{
            return logera_fd{open(path_.data(), O_RDONLY)};
        });
    using logera_check_open = decltype([](logera_fd& fd_) -> bool{
            return fd_.get() != -1;
        });
    using logera_fadvise = decltype([](logera_fd& fd_){
            posix_fadvise(fd_.get(), 0, 0, POSIX_FADV_SEQUENTIAL);
        });
    using logera_read = decltype([](logera_fd& fd_, char* buffer_, size_t size_) -> size_t{
            return read(fd_.get(), buffer_, size_);
        });
    using logera_check_read_error = decltype([](logera_fd&, size_t bytes_read_) -> bool{
            return bytes_read_ == size_t(-1);
        });
    using logera_check_read_eof = decltype([](logera_fd&, size_t bytes_read_) -> bool{
            return bytes_read_ == 0;
    });
    } // namespace

#else // C standard library version
    #include <cstdio>

    namespace{
    using logera_fd = std::unique_ptr<std::FILE, decltype([](std::FILE* fd_){ std::fclose(fd_); })>;
    using logera_open = decltype([](pmt::path_view path_) -> logera_fd{
            return logera_fd{std::fopen(path_.data(), "rb")};
        });
    using logera_check_open = decltype([](logera_fd& fd_) -> bool{
            return fd_ != nullptr;
        });
    using logera_fadvise = decltype([](logera_fd& fd_){});
    using logera_read = decltype([](logera_fd& fd_, char* buffer_, size_t size_) -> size_t{
            return std::fread(buffer_, 1, size_, fd_.get());
        });
    using logera_check_read_error = decltype([](logera_fd& fd_, size_t) -> bool{
            return std::ferror(fd_.get());
        });
    using logera_check_read_eof = decltype([](logera_fd& fd_, size_t) -> bool{
            return std::feof(fd_.get());
        });
    } // namespace
#endif
// clang-format on

namespace pmt
{
file_reader::file_reader(i_warning_collector& warning_collector_)
  : _warning_collector{ warning_collector_ }
  , _buffer{}
{
}

auto file_reader::read(path_view path_) -> std::string_view
{
  size_t const chunk_size = 1024 * 8;
  logera_fd    fd         = logera_open{}(path_);

  if (!logera_check_open{}(fd))
  {
    _warning_collector.add_warning(CANNOT_OPEN_FILE, warning::location{ path_ }, "Cannot open file");
    return "";
  }

  logera_fadvise{}(fd);

  _buffer.resize(chunk_size);

  size_t total_bytes_read = 0;

  while (size_t const bytes_read = logera_read{}(fd, _buffer.data() + total_bytes_read, chunk_size))
  {
    total_bytes_read += bytes_read;

    if (logera_check_read_error{}(fd, bytes_read))
    {
      _warning_collector.add_warning(FAILED_READING_FILE, warning::location{ path_ }, "Failed reading file");
      return "";
    }

    if (logera_check_read_eof{}(fd, bytes_read))
      break;

    _buffer.resize(total_bytes_read + chunk_size);
  }

  // Shrink the buffer to how much was actually read
  _buffer.resize(total_bytes_read);

  return string_view{ _buffer };
}
} // namespace pmt