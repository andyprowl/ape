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
        : GpuTimeMetrics{Nanoseconds{}}
    {
    }

    explicit GpuTimeMetrics(Nanoseconds const duration)
        : duration{duration}
    {
    }

    // virtual (from TaskProfileMetrics)
    auto clone() const
        -> std::unique_ptr<basix::TaskProfileMetrics> override
    {
        return std::make_unique<GpuTimeMetrics>(duration);
    }

public:

    Nanoseconds duration;

};

} // namespace ape
