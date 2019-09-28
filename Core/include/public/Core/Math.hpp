#pragma once

#include <algorithm>

namespace ape
{

template<typename T>
auto clamp(T const value, T const min, T const max)
    -> T
{
    return std::max(min, std::min(value, max));
}

} // namespace ape
