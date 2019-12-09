#pragma once

#include <Ape/Rendering/GpuProfiling/QueryPool.hpp>

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
