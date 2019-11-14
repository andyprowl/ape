#pragma once

#include <iterator>
#include <type_traits>

namespace basix
{

template<typename T>
using EnableIfLValue = std::enable_if_t<std::is_reference_v<T>>;

template<typename T>
using EnableIfRValue = std::enable_if_t<!std::is_reference_v<T>>;

template<typename Target, typename Source>
auto moveAppend(Target & target, Source && source)
    -> void
{
    target.insert(
        std::end(target),
        std::make_move_iterator(std::begin(source)),
        std::make_move_iterator(std::end(source)));
}

template<typename Target, typename Source, EnableIfRValue<Source> * = nullptr>
auto append(Target & target, Source && source)
    -> void
{
    moveAppend(target, std::move(source));
}

template<typename Target, typename Source, EnableIfLValue<Source> * = nullptr>
auto append(Target & target, Source && source)
    -> void
{
    target.insert(std::end(target), std::begin(source), std::end(source));
}

} // namespace basix
