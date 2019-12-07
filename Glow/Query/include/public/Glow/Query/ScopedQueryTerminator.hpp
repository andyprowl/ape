#pragma once

#include <Glow/Query/ScopedQuery.hpp>

namespace glow
{

class [[nodiscard]] ScopedQueryTerminator
{

public:

    explicit ScopedQueryTerminator(ScopedQuery & query)
        : query{&query}
    {
    }

    ScopedQueryTerminator(ScopedQueryTerminator const & rhs) = delete;

    ScopedQueryTerminator(ScopedQueryTerminator && rhs) noexcept
        : query{std::exchange(rhs.query, nullptr)}
    {
    }

    auto operator = (ScopedQueryTerminator const & rhs)
        -> ScopedQueryTerminator & = delete;

    auto operator = (ScopedQueryTerminator && rhs) noexcept
        -> ScopedQueryTerminator &
    {
        query = std::exchange(rhs.query, nullptr);

        return *this;
    }

    ~ScopedQueryTerminator()
    {
        if (query != nullptr)
        {
            query->end();
        }
    }

private:

    ScopedQuery * query;

};

} // namespace glow
