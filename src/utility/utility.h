//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <memory>
#include <type_traits>
#include <string>

/*
    This function shuffles the elements of the range [data_begin, data_end)
    according to the indices in the range [indices_begin, indices_end).

    Because it is intended to be used in indirect sorting, where the indices
    are often calculated once and then reused to sort many ranges, the indices
    are stored in a buffer and reused if provided.
*/

template <typename T, typename U>
    requires(std::is_integral_v<typename U::value_type>)
auto indirect_rearrange(T data_begin, T data_end, U indices_begin, std::unique_ptr<typename U::value_type[]> buf);

#include "indirect_rearrange-inl.h" // implementation

/*
    Replaces all occurance of to_erase in target with to_replace iteratively.
*/
void erase_and_replace(std::string* target, std::string const& to_erase, std::string const& to_replace);

/*
    Reads the file into a buffer and returns the buffer.
    The size including the null terminator is stored in size.

*/
auto read_file(char const* filename, uint32_t* size) -> std::unique_ptr<char[]>;

/*
    Returns the line number of the character at char_index in buffer.
*/
auto count_line_nr(char const* buffer, uint32_t char_index) -> uint32_t;