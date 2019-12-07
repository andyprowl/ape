#pragma once

#include <Basix/Profiling/ProfiledTaskBuilder.hpp>
#include <Basix/Profiling/ScopedCpuTimeProfiling.hpp>

namespace basix
{

class CpuTimeTaskProfiler : private ProfiledTaskBuilder
{

public:

    auto startTimingNewTask(std::string_view name, std::string_view description = "")
        -> ScopedCpuTimeProfiling;

    auto getProfiledTask()
        -> ProfiledTask &;

};

} // namespace basix
