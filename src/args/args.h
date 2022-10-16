#ifndef INCLUDED_ARGS_H
#define INCLUDED_ARGS_H

/*
*    A simplified version of the Arg class found in lib bobcat
*/

#include "../sparse_array/sparse_array.h"

#include <string>

class Args {
    SparseArray<uint8_t>     d_types;
    SparseArray<std::string> d_vals;

public:
    Args(char const* optNTBS, char const* const* argv);

    // returns value of the option (can be an empty string). Sets 'wasSpecified' to true if the option
    // was specified, otherwise the returned string is empty. 'wasSpecified' must not be a nullptr.
    std::string const& option(bool* wasSpecified, char option) const;

private:
    void setTypes(char const* optNTBS);
    void setVals(char const* const* argv);
};

#endif
