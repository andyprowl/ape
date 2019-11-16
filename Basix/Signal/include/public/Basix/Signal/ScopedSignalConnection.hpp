#pragma once

#include <functional>

namespace basix
{

class [[nodiscard]] ScopedSignalConnection
{
    
public:

    explicit ScopedSignalConnection(std::function<void()> disconnector);

    ScopedSignalConnection(ScopedSignalConnection const & rhs) = delete;

    ScopedSignalConnection(ScopedSignalConnection && rhs) noexcept;

    auto operator = (ScopedSignalConnection const & rhs)
        -> ScopedSignalConnection & = delete;

    auto operator = (ScopedSignalConnection && rhs) noexcept
        -> ScopedSignalConnection &;
    
    ~ScopedSignalConnection();

    auto detach()
        -> void;

    auto disconnect()
        -> void;

private:

    std::function<void()> disconnector;

};

} // namespace basix
