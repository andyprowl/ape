#pragma once

#include <string_view>

namespace basix
{

auto beginsWith(std::string_view s, std::string_view suffix)
    -> bool;

auto endsWith(std::string_view s, std::string_view suffix)
    -> bool;

} // namespace basix
