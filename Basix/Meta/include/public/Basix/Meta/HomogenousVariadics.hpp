#pragma once

#include <Basix/Meta/EnableIf.hpp>
#include <Basix/Meta/TypeListPredicates.hpp>

#define variadicListOf(T, Ts) \
    typename... Ts, \
    basix::EnableIf<basix::allTypesEqual<T, Ts...>> * = nullptr

#define staticListOf(N, T, Ts) \
    typename... Ts, \
    basix::EnableIf<basix::allTypesEqual<T, Ts...> && (sizeof...(Ts) == N)> * = nullptr
