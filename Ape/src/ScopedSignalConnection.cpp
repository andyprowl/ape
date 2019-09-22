#include <Ape/ScopedSignalConnection.hpp>

namespace ape
{

ScopedSignalConnection::ScopedSignalConnection(std::function<void()> disconnector)
    : disconnector{std::move(disconnector)}
{
}

auto ScopedSignalConnection::detach()
    -> void
{
    disconnector = std::function<void()>{};
}

ScopedSignalConnection::~ScopedSignalConnection()
{
    if (disconnector)
    {
        disconnector();
    }
}

} // namespace ape
