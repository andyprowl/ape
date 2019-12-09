#include <Ape/Rendering/GpuProfiling/TimestampQueryPool.hpp>

namespace ape
{

static auto const disableGpuTimeProfiling = false;

TimestampQueryPool::TimestampQueryPool(int const numOfPreCreatedQueries)
    : queries{numOfPreCreatedQueries}
{
}

auto TimestampQueryPool::placeTimestampRecordingRequest(ResultReceiver resultReceiver)
    -> void
{
    if (disableGpuTimeProfiling)
    {
        return;
    }

    auto query = fetchAvailableQuery();

    query.scheduleTimestampRecording();

    recordings.emplace_back(std::move(query), std::move(resultReceiver));
}

auto TimestampQueryPool::fetchAndStoreAllResults()
    -> void
{
    for (auto & recording : recordings)
    {
        auto const result = recording.query.waitForResultAndFetch();

        recording.resultReceiver(result);

        queries.push_back(std::move(recording.query));
    }
    
    recordings.clear();
}

auto TimestampQueryPool::fetchAvailableQuery()
    -> glow::TimestampQuery
{
    if (queries.empty())
    {
        return glow::TimestampQuery{};
    }

    auto query = std::move(queries.back());

    queries.pop_back();

    return query;
}

} // namespace ape
