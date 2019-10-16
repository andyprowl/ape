#pragma once

#include <Foundational/Meta/EnableIf.hpp>
#include <Foundational/Meta/TypeListPredicates.hpp>

#define variadicListOf(T, Ts) \
    typename... Ts, EnableIf<allTypesEqual<T, Ts...>> * = nullptr

#define staticListOf(N, T, Ts) \
    typename... Ts, EnableIf<allTypesEqual<T, Ts...> && (sizeof...(Ts) == N)> * = nullptr
