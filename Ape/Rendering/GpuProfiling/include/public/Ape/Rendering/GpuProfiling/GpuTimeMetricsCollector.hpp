#pragma once

#include <Ape/Rendering/GpuProfiling/GpuTimeMetrics.hpp>
#include <Ape/Rendering/GpuProfiling/TimestampQueryPool.hpp>

#include <utility>

namespace ape
{

class GpuTimeMetricsCollector
{

public:

    using Nanoseconds = GpuTimeMetrics::Nanoseconds;

public:

    explicit GpuTimeMetricsCollector(TimestampQueryPool & queryPool)
        : queryPool{&queryPool}
        , metrics{std::make_unique<GpuTimeMetrics>()}
    {
        scheduleStartTimestampRecording();
    }

    auto extractMetrics()
        -> std::unique_ptr<GpuTimeMetrics>
    {
        scheduleEndTimestampRecording();

        return std::move(metrics);
    }

private:

    auto scheduleStartTimestampRecording()
        -> void
    {
        auto const resultReceiver = [m = metrics.get()] (Nanoseconds const result)
        {
            m->setStartTimestamp(result);
        };

        queryPool->placeTimestampRecordingRequest(resultReceiver);
    }

    auto scheduleEndTimestampRecording()
        -> void
    {
        auto const resultReceiver = [m = metrics.get()] (Nanoseconds const result)
        {
            m->setEndTimestamp(result);
        };

        queryPool->placeTimestampRecordingRequest(resultReceiver);
    }

private:

    TimestampQueryPool * queryPool;

    std::unique_ptr<GpuTimeMetrics> metrics;

};

} // namespace ape
