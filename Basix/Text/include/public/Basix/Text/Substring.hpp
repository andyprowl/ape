#pragma once

#include <string_view>

namespace basix
{

auto endsWith(std::string_view s, std::string_view suffix)
    -> bool;

} // namespace basix
