#pragma once

#include <Ape/Rendering/GpuProfiling/ElapsedTimeQueryPool.hpp>
#include <Ape/Rendering/GpuProfiling/ScopedCpuGpuTimeProfiling.hpp>
#include <Ape/Rendering/GpuProfiling/TimestampQueryPool.hpp>

#include <Basix/Profiling/CpuTimeMetricsCollector.hpp>
#include <Basix/Profiling/ScopedCpuTimeProfiling.hpp>
#include <Basix/Profiling/TaskProfiler.hpp>

namespace ape
{

class TaskTimeProfiler : public basix::TaskProfiler
{

public:

    TaskTimeProfiler()
        : doNotProfile{false}
    {
    }

    auto startTimingCpuTask(
        std::string_view const name,
        std::string_view const description = "")
        -> basix::ScopedCpuTimeProfiling
    {
        if (doNotProfile)
        {
            return {};
        }

        return TaskProfiler::openNewTaskProfile(
            name,
            description,
            basix::CpuTimeMetricsCollector{});
    }

    template<GpuTimeQueryType QueryType = GpuTimeQueryType::timestamp>
    auto startTimingCpuGpuTask(
        std::string_view const name,
        std::string_view const description = "")
        -> ScopedCpuGpuTimeProfiling<QueryType>
    {
        if (doNotProfile)
        {
            return {};
        }

        if constexpr (QueryType == GpuTimeQueryType::timestamp)
        {
            return TaskProfiler::openNewTaskProfile(
                name,
                description,
                basix::CpuTimeMetricsCollector{},
                GpuTimestampMetricsCollector{timestampQueryPool});
        }
        else
        {
            return TaskProfiler::openNewTaskProfile(
                name,
                description,
                basix::CpuTimeMetricsCollector{},
                GpuElapsedTimeMetricsCollector{elapsedTimeQueryPool});
        }
    }

    auto fetchGpuTimingResults()
        -> void
    {
        if (doNotProfile)
        {
            return;
        }

        timestampQueryPool.fetchAndStoreAllResults();

        elapsedTimeQueryPool.fetchAndStoreAllResults();
    }

    auto disableProfiling()
        -> void
    {
        doNotProfile = true;
    }

    auto enableProfiling()
        -> void
    {
        doNotProfile = false;
    }

    auto isProfilingDisabled() const
        -> bool
    {
        return doNotProfile;
    }

private:

    bool doNotProfile;

    TimestampQueryPool timestampQueryPool;

    ElapsedTimeQueryPool elapsedTimeQueryPool;

};

} // namespace ape
