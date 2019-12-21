#pragma once

#include <Ape/Engine/FrameProfiling/GpuTimeMetrics.hpp>
#include <Ape/Engine/FrameProfiling/TimestampQueryPool.hpp>

#include <memory>

namespace ape
{

class GpuTimestampMetricsCollector
{

public:

    using Nanoseconds = GpuTimeMetrics::Nanoseconds;

public:

    explicit GpuTimestampMetricsCollector(TimestampQueryPool & queryPool)
        : queryPool{&queryPool}
        , metrics{std::make_unique<GpuTimeMetrics>()}
        , startTime{scheduleStartTimestampRecording()}
    {
    }

    auto extractMetrics()
        -> std::unique_ptr<GpuTimeMetrics>
    {
        scheduleEndTimestampRecording();

        return std::move(metrics);
    }

private:

    auto scheduleStartTimestampRecording()
        -> std::shared_ptr<Nanoseconds>
    {
        auto resultHolder = std::make_shared<Nanoseconds>();

        auto const resultReceiver = [resultHolder] (Nanoseconds const result)
        {
            *resultHolder = result;
        };

        queryPool->placeTimestampRecordingRequest(resultReceiver);

        return resultHolder;
    }

    auto scheduleEndTimestampRecording()
        -> void
    {
        auto const resultReceiver = [m = metrics.get(), st = startTime] (Nanoseconds const result)
        {
            m->duration = (result - *st);
        };

        queryPool->placeTimestampRecordingRequest(resultReceiver);
    }

private:

    TimestampQueryPool * queryPool;

    std::unique_ptr<GpuTimeMetrics> metrics;

    std::shared_ptr<Nanoseconds> startTime;

};

} // namespace ape
