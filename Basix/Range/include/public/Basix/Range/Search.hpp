#pragma once

#include <algorithm>
#include <functional>
#include <iterator>

namespace basix
{

template<typename Container, typename V>
auto find(Container const & container, V const & value)
{
    return std::find(std::cbegin(container), std::cend(container), value);
}

template<typename Container, typename Pred>
auto findIf(Container const & container, Pred pred)
{
    return std::find_if(std::cbegin(container), std::cend(container), std::move(pred));
}

template<typename Container, typename V>
auto contains(Container const & container, V const & value)
    -> bool
{
    auto const it = find(container, value);
    
    return (it != std::cend(container));
}

template<typename Container, typename Pred>
auto containsIf(Container const & container, Pred pred)
    -> bool
{
    auto const it = findIf(container, std::move(pred));
    
    return (it != std::cend(container));
}

template<typename Container, typename Pred>
auto noneOf(Container const & container, Pred pred)
    -> bool
{
    return !containsIf(container, pred);
}

template<typename Container, typename Pred>
auto allOf(Container const & container, Pred pred)
    -> bool
{
    return noneOf(container, std::negate{pred});
}

} // namespace basix
