#pragma once

#include <Ape/Rendering/GpuProfiling/GpuTimeMetricsCollector.hpp>

#include <Basix/Profiling/CpuTimeMetricsCollector.hpp>
#include <Basix/Profiling/ScopedTaskProfiling.hpp>

namespace ape
{

class ScopedTimeProfiling : public basix::ScopedTaskProfiling<
    basix::CpuTimeMetricsCollector,
    GpuTimeMetricsCollector>
{

public:

    using ScopedTaskProfiling::ScopedTaskProfiling;

    ScopedTimeProfiling(ScopedTaskProfiling profiling)
        : ScopedTaskProfiling{std::move(profiling)}
    {
    }

};

} // namespace ape
