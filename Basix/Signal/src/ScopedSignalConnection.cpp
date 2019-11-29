#include <Basix/Signal/ScopedSignalConnection.hpp>

#include <utility>

namespace basix
{

ScopedSignalConnection::ScopedSignalConnection(std::function<void()> disconnector)
    : disconnector{std::move(disconnector)}
{
}

ScopedSignalConnection::ScopedSignalConnection(ScopedSignalConnection && rhs) noexcept
    : disconnector{std::exchange(rhs.disconnector, nullptr)}
{
}

auto ScopedSignalConnection::operator = (ScopedSignalConnection && rhs) noexcept
    -> ScopedSignalConnection &
{
    disconnect();

    disconnector = std::exchange(rhs.disconnector, nullptr);

    return *this;
}

auto ScopedSignalConnection::detach()
    -> void
{
    disconnector = nullptr;
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

        detach();
    }
}

} // namespace basix
