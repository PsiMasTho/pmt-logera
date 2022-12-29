#include "string_util.h"

using namespace std;

void eraseAndReplace(string* target, string const& toErase, string const& toReplace)
{
    size_t const pos = target->find(toErase);
    if(pos != string::npos)
    {
        target->erase(pos, toErase.size());
        target->insert(pos, toReplace);
    }
}

void appendToString(string& lhs, string_view const rhs, string const& delim)
{
    lhs += delim;
    lhs += rhs;
}