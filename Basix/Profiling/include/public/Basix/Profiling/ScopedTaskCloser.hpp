#pragma once

#include <utility>

namespace basix
{

class ProfiledTaskBuilder;

class [[nodiscard]] ScopedTaskCloser
{

public:

    ScopedTaskCloser()
        : builder{nullptr}
    {
    }

    explicit ScopedTaskCloser(ProfiledTaskBuilder & builder)
        : builder{&builder}
    {
    }

    ScopedTaskCloser(ScopedTaskCloser const & rhs) = delete;

    ScopedTaskCloser(ScopedTaskCloser && rhs) noexcept
        : builder{std::exchange(rhs.builder, nullptr)}
    {        
    }

    auto operator = (ScopedTaskCloser const & rhs)
        -> ScopedTaskCloser & = delete;

    auto operator = (ScopedTaskCloser && rhs) noexcept
        -> ScopedTaskCloser &
    {
        builder = std::exchange(rhs.builder, nullptr);

        return *this;
    }

    ~ScopedTaskCloser();

private:

    ProfiledTaskBuilder * builder;

};

} // namespace basix
