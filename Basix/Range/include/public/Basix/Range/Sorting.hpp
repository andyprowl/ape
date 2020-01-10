#pragma once

#include <algorithm>

namespace basix
{

template<typename Cont>
auto sort(Cont & c)
    -> void
{
    std::sort(std::begin(c), std::end(c));
}

template<typename Cont, typename Pred>
auto sort(Cont & c, Pred const p)
-> void
{
    std::sort(std::begin(c), std::end(c), p);
}

} // namespace basix
