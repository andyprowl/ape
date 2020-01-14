#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

namespace basix
{

template<typename T>
auto clamp(T const value, T const min, T const max)
    -> T
{
    return std::max(min, std::min(value, max));
}

// Taken from https://stackoverflow.com/a/11398748/1932150
inline auto log2(std::uint32_t value)
    -> int
{
    constexpr auto table = std::array<std::uint32_t, 32u>{{
         0,  9,  1, 10, 13, 21,  2, 29,
        11, 14, 16, 18, 22, 25,  3, 30,
         8, 12, 20, 28, 15, 17, 24,  7,
        19, 27, 23,  6, 26,  5,  4, 31}};

    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;

    return table[static_cast<std::uint32_t>(value * 0x07C4ACDD) >> 27];
}

} // namespace basix
