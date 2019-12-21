#pragma once

#include <Ape/Engine/FrameProfiling/GpuElapsedTimeMetricsCollector.hpp>
#include <Ape/Engine/FrameProfiling/GpuTimeQueryType.hpp>
#include <Ape/Engine/FrameProfiling/GpuTimestampMetricsCollector.hpp>

#include <Basix/Profiling/CpuTimeMetricsCollector.hpp>
#include <Basix/Profiling/ScopedTaskProfiling.hpp>

namespace ape
{

template<GpuTimeQueryType QueryType>
class ScopedCpuGpuTimeProfiling;

template<>
class ScopedCpuGpuTimeProfiling<GpuTimeQueryType::timestamp>
    : public basix::ScopedTaskProfiling<
        std::optional<basix::CpuTimeMetricsCollector>,
        std::optional<GpuTimestampMetricsCollector>>
{

public:

    using ScopedTaskProfiling::ScopedTaskProfiling;

    ScopedCpuGpuTimeProfiling(ScopedTaskProfiling profiling)
        : ScopedTaskProfiling{std::move(profiling)}
    {
    }

};

template<>
class ScopedCpuGpuTimeProfiling<GpuTimeQueryType::elapsedTime>
    : public basix::ScopedTaskProfiling<
        std::optional<basix::CpuTimeMetricsCollector>,
        std::optional<GpuElapsedTimeMetricsCollector>>
{

public:

    using ScopedTaskProfiling::ScopedTaskProfiling;

    ScopedCpuGpuTimeProfiling(ScopedTaskProfiling profiling)
        : ScopedTaskProfiling{std::move(profiling)}
    {
    }

};

} // namespace ape
