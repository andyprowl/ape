#pragma once

#include <Basix/Profiling/CpuTimeMetrics.hpp>

#include <utility>

namespace basix
{

class [[nodiscard]] CpuTimer
{

public:

    CpuTimer()
        : metrics{nullptr}
    {
    }

    explicit CpuTimer(CpuTimeMetrics & metrics)
        : metrics{&metrics}
    {
        metrics.cpuTiming.setStartTime(TaskTiming::Clock::now());
    }

    CpuTimer(CpuTimer const & rhs) = delete;

    CpuTimer(CpuTimer && rhs) noexcept
        : metrics{std::exchange(rhs.metrics, nullptr)}
    {
    }

    auto operator = (CpuTimer const & rhs)
        -> CpuTimer & = delete;

    auto operator = (CpuTimer && rhs) noexcept
        -> CpuTimer &
    {
        metrics = std::exchange(rhs.metrics, nullptr);

        return *this;
    }

    ~CpuTimer()
    {
        if (metrics != nullptr)
        {
            metrics->cpuTiming.setEndTime(TaskTiming::Clock::now());
        }
    }

private:

    CpuTimeMetrics * metrics;

};

} // namespace basix
