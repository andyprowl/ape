#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

namespace basix
{

template<typename T>
auto remove(std::vector<T> & v, T const & value)
    -> void
{
    auto const newEnd = std::remove(std::begin(v), std::end(v), value);

    v.erase(newEnd, std::cend(v));
}

template<typename T, typename Pred>
auto removeIf(std::vector<T> & v, Pred const pred)
    -> void
{
    auto const newEnd = std::remove_if(std::begin(v), std::end(v), pred);

    v.erase(newEnd, std::cend(v));
}

} // namespace basix
