#pragma once

#include <type_traits>

namespace basix
{

template<bool V>
using EnableIf = std::enable_if_t<V>;

} // namespace basix
