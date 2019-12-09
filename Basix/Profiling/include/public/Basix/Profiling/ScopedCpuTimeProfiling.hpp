#pragma once

#include <Basix/Profiling/CpuTimeMetricsCollector.hpp>
#include <Basix/Profiling/ScopedTaskProfiling.hpp>

namespace basix
{

class ScopedCpuTimeProfiling : public ScopedTaskProfiling<CpuTimeMetricsCollector>
{

public:

    using ScopedTaskProfiling::ScopedTaskProfiling;

    ScopedCpuTimeProfiling(ScopedTaskProfiling profiling)
        : ScopedTaskProfiling{std::move(profiling)}
    {
    }

};

} // namespace basix
