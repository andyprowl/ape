#include <Glow/Query/ScopedQuery.hpp>

#include <Basix/Memory/Null.hpp>

#include <glad/glad.h>

#include <array>
#include <atomic>
#include <cassert>

namespace glow
{

namespace
{

using AtomicQueryPointer = std::atomic<ScopedQuery const *>;

auto && activeQueries = std::array<AtomicQueryPointer, 6u>{{nullptr}};

auto getActiveQueryMarker(const ScopedQueryType type)
    -> AtomicQueryPointer &
{
    auto const glType = convertToOpenGLQueryType(type);

    return activeQueries[glType];
}

} // unnamed namespace


ScopedQuery::ScopedQuery(ScopedQueryType const type)
    : type{type}
{
}

auto ScopedQuery::begin()
    -> void
{
    activateQuery();

    auto const glType = convertToOpenGLQueryType(type);

    auto const id = getId();

    glBeginQuery(glType, id);
}

auto ScopedQuery::end()
    -> void
{
    assert(isActive());

    auto const glType = convertToOpenGLQueryType(type);

    glEndQuery(glType);
}

auto ScopedQuery::getType() const
    -> ScopedQueryType
{
    return type;
}

auto ScopedQuery::isActive() const
    -> bool
{
    auto const glType = convertToOpenGLQueryType(type);

    auto & activeQuery = activeQueries[glType];

    return (activeQuery == this);
}

auto ScopedQuery::activateQuery() const
    -> void
{
    auto & activeQuery = getActiveQueryMarker(type);

    auto expectedActiveQuery = basix::null_ptr<ScopedQuery const>;

    auto const canActivate = activeQuery.compare_exchange_strong(expectedActiveQuery, this);

    if (!canActivate)
    {
        throw QueryOfSameTypeStillActive{};
    }
}

} // namespace glow
