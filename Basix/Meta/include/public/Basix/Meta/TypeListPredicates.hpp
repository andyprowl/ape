#pragma once

#include <Basix/Meta/LogicalPredicates.hpp>

namespace basix
{

template<typename T, typename... Ts>
using AllTypesEqual = AllOf<std::is_same_v<T, Ts>...>;

template<typename T, typename... Ts>
auto allTypesEqual = AllTypesEqual<T, Ts...>{};

} // namespace basix

