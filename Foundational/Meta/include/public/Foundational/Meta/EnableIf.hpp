#pragma once

#include <type_traits>

namespace ape
{

template<bool V>
using EnableIf = std::enable_if_t<V>;

} // namespace ape
