#pragma once

#include <string_view>

namespace ape
{

auto endsWith(std::string_view s, std::string_view suffix)
    -> bool;

} // namespace ape
