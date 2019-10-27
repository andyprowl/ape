#pragma once

#include <algorithm>
#include <iterator>

namespace ape
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

} // namespace ape
