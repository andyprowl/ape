#pragma once

#include <type_traits>

namespace ape
{

template<bool... Vs>
class AllOf : public std::true_type
{
};

template<bool V, bool... Vs>
class AllOf<V, Vs...> : public std::integral_constant<bool, V && AllOf<Vs...>::value>
{
};

template<bool... Vs>
class AnyOf : public std::false_type
{
};

template<bool V, bool... Vs>
class AnyOf<V, Vs...> : public std::integral_constant<bool, V || AnyOf<Vs...>::value>
{
};

template<bool... Vs>
class NoneOf : public std::true_type
{
};

template<bool V, bool... Vs>
class NoneOf<V, Vs...> : public std::integral_constant<bool, !V && NoneOf<Vs...>::value>
{
};

} // namespace ape
