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

using AtomicResourceId = std::atomic<GpuResource::Id>;

auto const nullId = GpuResource::Id{};

auto && activeQueries = std::array<AtomicResourceId, 6u>{{nullId}};

auto getActiveQueryMarker(const ScopedQueryType type)
    -> AtomicResourceId &
{
    return activeQueries[static_cast<std::size_t>(type)];
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

    deactivateQuery();
}

auto ScopedQuery::getType() const
    -> ScopedQueryType
{
    return type;
}

auto ScopedQuery::isActive() const
    -> bool
{
    auto & activeQuery = getActiveQueryMarker(type);

    return (activeQuery == getId());
}

auto ScopedQuery::activateQuery() const
    -> void
{
    auto & activeQuery = getActiveQueryMarker(type);

    auto expectedActiveQueryId = nullId;

    auto const id = getId();

    auto const canActivate = activeQuery.compare_exchange_strong(expectedActiveQueryId, id);

    if (!canActivate)
    {
        throw QueryOfSameTypeStillActive{};
    }

    assert(activeQuery == id);
}

auto ScopedQuery::deactivateQuery() const
    -> void
{
    auto & activeQuery = getActiveQueryMarker(type);

    auto expectedActiveQueryId = getId();

    [[maybe_unused]]
    auto const deactivated = activeQuery.compare_exchange_strong(expectedActiveQueryId, nullId);

    assert(deactivated);
}

} // namespace glow
