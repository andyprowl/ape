#pragma once

#include <Basix/Profiling/TaskProfileMetrics.hpp>

#include <chrono>

namespace ape
{

class GpuTimeMetrics : public basix::TaskProfileMetrics
{

public:

    using Nanoseconds = std::chrono::nanoseconds;

public:

    GpuTimeMetrics()
        : GpuTimeMetrics{Nanoseconds{}, Nanoseconds{}}
    {
    }

    GpuTimeMetrics(Nanoseconds const startTimestamp, Nanoseconds const duration)
        : startTimestamp{startTimestamp}
        , duration{duration}
    {
    }

    // virtual (from TaskProfileMetrics)
    auto clone() const
        -> std::unique_ptr<TaskProfileMetrics> override
    {
        return std::make_unique<GpuTimeMetrics>(startTimestamp, duration);
    }

    auto setStartTimestamp(Nanoseconds const newStartTimestamp)
        -> void
    {
        startTimestamp = newStartTimestamp;
    }

    auto setEndTimestamp(Nanoseconds const newEndTimestamp)
        -> void
    {
        duration = (newEndTimestamp - startTimestamp);
    }

    auto getDuration() const
        -> Nanoseconds
    {
        return duration;
    }

private:

    Nanoseconds startTimestamp;

    Nanoseconds duration;

};

} // namespace ape
