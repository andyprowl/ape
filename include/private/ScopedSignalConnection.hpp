#pragma once

#include <functional>

class ScopedSignalConnection
{
    
public:

    explicit ScopedSignalConnection(std::function<void()> disconnector);

    ScopedSignalConnection(ScopedSignalConnection const & rhs) = delete;

    ScopedSignalConnection(ScopedSignalConnection && rhs) = default;

    auto operator = (ScopedSignalConnection const & rhs)
        -> ScopedSignalConnection & = delete;

    auto operator = (ScopedSignalConnection && rhs)
        -> ScopedSignalConnection & = default;

    auto detach()
        -> void;

    ~ScopedSignalConnection();

private:

    std::function<void()> disconnector;

};