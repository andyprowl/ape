#pragma once

#include <Basix/Profiling/TaskProfileMetrics.hpp>
#include <Basix/Profiling/TaskTiming.hpp>

namespace basix
{

class CpuTimeMetrics : public TaskProfileMetrics
{

public:

    CpuTimeMetrics() = default;

    explicit CpuTimeMetrics(TaskTiming const & cpuTiming)
        : cpuTiming{cpuTiming}
    {
    }

    auto clone() const
        -> std::unique_ptr<TaskProfileMetrics> override
    {
        return std::make_unique<CpuTimeMetrics>(cpuTiming);
    }

public:

    TaskTiming cpuTiming;

};

} // namespace basix
