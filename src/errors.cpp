#include "errors.hpp"

#include <cassert>

using namespace std;

namespace error
{

record::record(int code, ...)
    : msg(256, '\0')
    , code(code)
{
    assert(code < _CODE_COUNT);

    va_list args;
    va_start(args, code);
    vsnprintf(msg.data(), msg.size(), fmts[code], args);
    va_end(args);
}

}
