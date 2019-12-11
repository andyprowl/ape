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
        : TaskTimeProfiler{true, true}
    {
    }

    explicit TaskTimeProfiler(bool const collectCpuTime, bool const collectGpuTime)
        : collectCpuTime{collectCpuTime}
        , collectGpuTime{collectGpuTime}
    {
    }

    auto startTimingCpuTask(
        std::string_view const name,
        std::string_view const description = "")
        -> basix::ScopedCpuTimeProfiling
    {
        if (!collectCpuTime)
        {
            return basix::ScopedCpuTimeProfiling{};
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
        if (!collectCpuTime && !collectGpuTime)
        {
            return {};
        }

        if constexpr (QueryType == GpuTimeQueryType::timestamp)
        {
            return TaskProfiler::openNewTaskProfile(
                name,
                description,
                makeCpuTimeMetricsCollector(),
                makeGpuTimestampMetricsCollector());
        }
        else
        {
            return TaskProfiler::openNewTaskProfile(
                name,
                description,
                makeCpuTimeMetricsCollector(),
                makeGpuElapsedTimeMetricsCollector());
        }
    }

    auto fetchGpuTimingResults()
        -> void
    {
        if (!collectGpuTime)
        {
            return;
        }

        timestampQueryPool.fetchAndStoreAllResults();

        elapsedTimeQueryPool.fetchAndStoreAllResults();
    }

    auto isCpuProfilingEnabled() const
        -> bool
    {
        return collectCpuTime;
    }

    auto isGpuProfilingEnabled() const
        -> bool
    {
        return collectGpuTime;
    }

    auto enableCpuProfiling()
        -> void
    {
        collectCpuTime = true;
    }

    auto disableCpuProfiling()
        -> void
    {
        collectCpuTime = false;
    }

    auto enableGpuProfiling()
        -> void
    {
        collectGpuTime = true;
    }

    auto disableGpuProfiling()
        -> void
    {
        collectGpuTime = false;
    }

    auto enableProfiling()
        -> void
    {
        collectCpuTime = true;

        collectGpuTime = true;
    }

    auto disableProfiling()
        -> void
    {
        collectCpuTime = false;

        collectGpuTime = false;
    }

private:

    auto makeCpuTimeMetricsCollector()
        -> std::optional<basix::CpuTimeMetricsCollector>
    {
        if (isCpuProfilingEnabled())
        {
            return basix::CpuTimeMetricsCollector{};
        }
        else
        {
            return std::nullopt;
        }
    }

    auto makeGpuTimestampMetricsCollector()
        -> std::optional<GpuTimestampMetricsCollector>
    {
        if (isGpuProfilingEnabled())
        {
            return GpuTimestampMetricsCollector{timestampQueryPool};
        }
        else
        {
            return std::nullopt;
        }
    }

    auto makeGpuElapsedTimeMetricsCollector()
        -> std::optional<GpuElapsedTimeMetricsCollector>
    {
        if (isGpuProfilingEnabled())
        {
            return GpuElapsedTimeMetricsCollector{elapsedTimeQueryPool};
        }
        else
        {
            return std::nullopt;
        }
    }


private:

    bool collectCpuTime;

    bool collectGpuTime;

    TimestampQueryPool timestampQueryPool;

    ElapsedTimeQueryPool elapsedTimeQueryPool;

};

} // namespace ape
