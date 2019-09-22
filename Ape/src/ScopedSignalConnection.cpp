#include <Ape/ScopedSignalConnection.hpp>

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
