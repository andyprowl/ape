#pragma once

#include <Meta/EnableIf.hpp>
#include <Meta/TypeListPredicates.hpp>

#define variadicListOf(T, Ts) \
    typename... Ts, EnableIf<allTypesEqual<T, Ts...>> * = nullptr

#define staticListOf(N, T, Ts) \
    typename... Ts, EnableIf<allTypesEqual<T, Ts...> && (sizeof...(Ts) == N)> * = nullptr
