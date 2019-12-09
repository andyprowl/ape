#pragma once

#include <Ape/Rendering/GpuProfiling/ScopedTimeProfiling.hpp>
#include <Ape/Rendering/GpuProfiling/TimestampQueryPool.hpp>

#include <Basix/Profiling/CpuTimeMetricsCollector.hpp>
#include <Basix/Profiling/TaskProfiler.hpp>

namespace ape
{

class TaskTimeProfiler : public basix::TaskProfiler
{

public:

    auto startTimingNewTask(
        std::string_view const name,
        std::string_view const description = "")
        -> ScopedTimeProfiling
    {
        return TaskProfiler::openNewTaskProfile(
            name,
            description,
            basix::CpuTimeMetricsCollector{},
            GpuTimeMetricsCollector{queryPool});
    }

    auto fetchGpuTimingResults()
        -> void
    {
        queryPool.fetchAndStoreAllResults();
    }

private:

    TimestampQueryPool queryPool;

};

} // namespace ape
