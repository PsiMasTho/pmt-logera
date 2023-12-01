#pragma once

#include <cstdint>
#include <memory>
#include <utility>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using tok_t = i16; // underlying token type

using buffer_t = std::pair<std::unique_ptr<char[]>, u32>; // buffer, size

using source_location_t = u32; // offset into buffer

using tok_rec_idx_t = u32; // index into token record array