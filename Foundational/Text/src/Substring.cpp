#include <Foundational/Text/Substring.hpp>

namespace ape
{

auto endsWith(std::string_view s, std::string_view suffix)
    -> bool
{
    return (s.compare(s.length() - suffix.length(), suffix.length(), suffix) == 0);
}

} // namespace ape
