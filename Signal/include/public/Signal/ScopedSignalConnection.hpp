#pragma once

#include <functional>

namespace ape
{

// [[nodiscard]]
class ScopedSignalConnection
{
    
public:

    explicit ScopedSignalConnection(std::function<void()> disconnector);

    ScopedSignalConnection(ScopedSignalConnection const & rhs) = delete;

    ScopedSignalConnection(ScopedSignalConnection && rhs) noexcept;

    auto operator = (ScopedSignalConnection const & rhs)
        -> ScopedSignalConnection & = delete;

    auto operator = (ScopedSignalConnection && rhs) noexcept
        -> ScopedSignalConnection &;

    auto detach()
        -> void;

    ~ScopedSignalConnection();

private:

    auto disconnect()
        -> void;

private:

    std::function<void()> disconnector;

};

} // namespace ape
