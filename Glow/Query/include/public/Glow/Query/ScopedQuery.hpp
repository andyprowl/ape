#pragma once

#include <Glow/Query/Query.hpp>
#include <Glow/Query/ScopedQueryType.hpp>

namespace glow
{

class QueryOfSameTypeStillActive : public std::exception
{

public:

    // virtual (from std::exception)
    auto what() const noexcept
        -> const char * override
    {
        return "Could not begin query because another query of the same kind is still active";
    }

};

class ScopedQuery : public Query
{

public:

    explicit ScopedQuery(ScopedQueryType type);

    auto getType() const
        -> ScopedQueryType;

    auto isActive() const
        -> bool;

    auto begin()
        -> void;

    auto end()
        -> void;

private:

    auto activateQuery() const
        -> void;

private:

    ScopedQueryType type;

};

} // namespace glow
