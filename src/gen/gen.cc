#include "gen.h"

using namespace std;

#if __unix__
#    include "read_file_unix-inl.h"
#else
#    include "read_file_portable-inl.h"
#endif

auto read_file(char const* filename) -> buffer_t
{
#if __unix__
    return read_file_unix(filename);
#else
    return read_file_portable(filename);
#endif
}