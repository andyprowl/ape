#pragma once

#include <algorithm>
#include <vector>

namespace ape
{

template<typename Container, typename V>
auto find(Container const & container, V const & value)
{
    return std::find(std::cbegin(container), std::cend(container), value);
}

template<typename Container, typename V>
auto contains(Container const & container, V const & value)
    -> bool
{
    auto const it = find(container, value);
    
    return (it != std::cend(container));
}

template<typename T>
auto remove(std::vector<T> & v, T const & value)
    -> void
{
    auto const newEnd = std::remove(std::begin(v), std::end(v), value);

    v.erase(newEnd, std::cend(v));
}

template<template<typename...> class OutputContainer = std::vector, typename Container, typename F>
auto transform(Container const & container, F f)
    -> OutputContainer<std::decay_t<decltype(f(container[0]))>>
{
    auto result = OutputContainer<std::decay_t<decltype(f(container[0]))>>{};

    result.reserve(container.size());

    std::transform(std::cbegin(container), std::cend(container), std::back_inserter(result), f);

    return result;
}

} // namespace ape
