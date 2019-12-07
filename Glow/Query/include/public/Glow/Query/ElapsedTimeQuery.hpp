#pragma once

#include <Glow/Query/ScopedQuery.hpp>

#include <chrono>

namespace glow
{

class ElapsedTimeQuery : public ScopedQuery
{

public:

    using Nanoseconds = std::chrono::nanoseconds;

public:

    ElapsedTimeQuery();

    auto waitForResultAndFetch() const
        -> Nanoseconds;

    auto fetchResultIfAvailable() const
        -> std::optional<Nanoseconds>;

};

} // namespace glow
