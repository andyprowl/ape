#include <Ape/Engine/FrameProfiling/TimestampQueryPool.hpp>

namespace ape
{

namespace
{

auto const disableTimestampQueryPlacement = false;

} // unnamed namespace

auto TimestampQueryPool::placeTimestampRecordingRequest(ResultReceiver resultReceiver)
    -> void
{
    if (disableTimestampQueryPlacement)
    {
        return;
    }

    auto query = fetchAvailableQuery();

    query.scheduleTimestampRecording();

    recordings.emplace_back(std::move(query), std::move(resultReceiver));
}

} // namespace ape
