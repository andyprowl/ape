#pragma once

#include <Ape/Engine/FrameProfiling/QueryPool.hpp>

#include <Glow/Query/TimestampQuery.hpp>

namespace ape
{

class TimestampQueryPool : public QueryPool<glow::TimestampQuery>
{

public:

    using QueryPool::QueryPool;

    auto placeTimestampRecordingRequest(ResultReceiver resultReceiver)
        -> void;

};

} // namespace ape
