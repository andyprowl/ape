#pragma once

#include <Basix/Profiling/TaskProfileMetrics.hpp>

#include <chrono>

namespace basix
{

class CpuTimeMetrics : public TaskProfileMetrics
{

public:

    using Nanoseconds = std::chrono::nanoseconds;

public:

    CpuTimeMetrics() = default;

    explicit CpuTimeMetrics(Nanoseconds const duration)
        : duration{duration}
    {
    }

    // virtual (from TaskProfileMetrics)
    auto clone() const
        -> std::unique_ptr<TaskProfileMetrics> override
    {
        return std::make_unique<CpuTimeMetrics>(duration);
    }

public:

    Nanoseconds duration;

};

} // namespace basix
