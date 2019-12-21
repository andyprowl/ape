#include <Ape/Engine/FrameProfiling/ElapsedTimeQueryPool.hpp>

namespace ape
{

namespace
{

auto const disableElapsedTimeQueryPlacement = false;

} // unnamed namespace

auto ElapsedTimeQueryPool::beginQuery(ResultReceiver resultReceiver)
    -> QueryRegistration
{
    if (disableElapsedTimeQueryPlacement)
    {
        return -1;
    }

    auto query = fetchAvailableQuery();

    query.begin();

    auto const queryIndex = static_cast<int>(recordings.size());

    recordings.emplace_back(std::move(query), std::move(resultReceiver));

    return queryIndex;
}

auto ElapsedTimeQueryPool::endQuery(QueryRegistration const registration)
    -> void
{
    if (registration < 0)
    {
        return;
    }

    auto const queryIndex = static_cast<int>(registration);

    recordings[queryIndex].query.end();
}

} // namespace ape
