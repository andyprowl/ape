#pragma once

#include <Basix/Profiling/CpuTimer.hpp>
#include <Basix/Profiling/ScopedTaskCloser.hpp>

namespace basix
{

class [[nodiscard]] ScopedCpuTimeProfiling : private ScopedTaskCloser, private CpuTimer
{

public:

    ScopedCpuTimeProfiling() = default;

    ScopedCpuTimeProfiling(ScopedTaskCloser taskCloser, CpuTimer timer)
        : ScopedTaskCloser{std::move(taskCloser)}
        , CpuTimer{std::move(timer)}
    {
    }

};

} // namespace basix
