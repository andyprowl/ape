#pragma once

#include <Basix/Profiling/CpuTimeMetrics.hpp>

#include <chrono>

namespace basix
{

class CpuTimeMetricsCollector
{

public:

    using Clock = std::chrono::high_resolution_clock;

    using TimePoint = Clock::time_point;

    using Nanoseconds = CpuTimeMetrics::Nanoseconds;

public:

    CpuTimeMetricsCollector()
        : startTime{Clock::now()}
    {
    }

    auto extractMetrics()
        -> std::unique_ptr<CpuTimeMetrics>
    {
        auto const endTime = Clock::now();

        auto const duration = (endTime - startTime);

        return std::make_unique<CpuTimeMetrics>(duration);
    }

private:

    TimePoint startTime;

};

} // namespace basix
