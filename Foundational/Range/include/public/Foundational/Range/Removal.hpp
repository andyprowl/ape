#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

namespace ape
{


template<typename T>
auto remove(std::vector<T> & v, T const & value)
    -> void
{
    auto const newEnd = std::remove(std::begin(v), std::end(v), value);

    v.erase(newEnd, std::cend(v));
}

} // namespace ape
