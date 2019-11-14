#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

namespace basix
{

template<template<typename...> class OutputContainer = std::vector, typename Container, typename F>
auto transform(Container const & container, F f)
    -> OutputContainer<std::decay_t<decltype(f(container[0]))>>
{
    auto result = OutputContainer<std::decay_t<decltype(f(container[0]))>>{};

    result.reserve(container.size());

    std::transform(std::cbegin(container), std::cend(container), std::back_inserter(result), f);

    return result;
}

template<template<typename...> class OutputContainer = std::vector, typename Container, typename F>
auto transform(Container && container, F f)
    -> OutputContainer<std::decay_t<decltype(f(container[0]))>>
{
    auto result = OutputContainer<std::decay_t<decltype(f(container[0]))>>{};

    result.reserve(container.size());

    std::transform(std::begin(container), std::end(container), std::back_inserter(result), f);

    return result;
}

} // namespace basix
