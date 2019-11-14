#include <Basix/Signal/ScopedSignalConnection.hpp>

namespace basix
{

ScopedSignalConnection::ScopedSignalConnection(std::function<void()> disconnector)
    : disconnector{std::move(disconnector)}
{
}

ScopedSignalConnection::ScopedSignalConnection(ScopedSignalConnection && rhs) noexcept
    : disconnector{std::move(disconnector)}
{
    rhs.detach();
}

auto ScopedSignalConnection::operator = (ScopedSignalConnection && rhs) noexcept
    -> ScopedSignalConnection &
{
    disconnect();

    disconnector = std::move(disconnector);

    rhs.detach();

    return *this;
}

auto ScopedSignalConnection::detach()
    -> void
{
    disconnector = std::function<void()>{};
}

ScopedSignalConnection::~ScopedSignalConnection()
{
    disconnect();
}

auto ScopedSignalConnection::disconnect()
    -> void
{
    if (disconnector)
    {
        disconnector();
    }
}

} // namespace basix
