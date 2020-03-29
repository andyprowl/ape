#include <Basix/Text/Substring.hpp>

namespace basix
{

auto beginsWith(std::string_view s, std::string_view suffix)
    -> bool
{
    return (s.compare(0, suffix.length(), suffix) == 0);
}

auto endsWith(std::string_view s, std::string_view suffix)
    -> bool
{
    return (s.compare(s.length() - suffix.length(), suffix.length(), suffix) == 0);
}

} // namespace basix
