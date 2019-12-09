#pragma once

#include <Basix/Profiling/CpuTimeMetricsCollector.hpp>
#include <Basix/Profiling/ScopedCpuTimeProfiling.hpp>
#include <Basix/Profiling/TaskProfiler.hpp>

namespace basix
{

class CpuTimeTaskProfiler : public TaskProfiler
{

public:

    auto startTimingNewTask(std::string_view name, std::string_view description = "")
        -> ScopedCpuTimeProfiling
    {
        return TaskProfiler::openNewTaskProfile(name, description, CpuTimeMetricsCollector{});
    }

private:

    TaskProfiler * taskBuilder;

};

} // namespace basix
