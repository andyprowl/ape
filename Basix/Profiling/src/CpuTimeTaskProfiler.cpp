#include <Basix/Profiling/CpuTimeTaskProfiler.hpp>

#include <Basix/Profiling/CpuTimeMetrics.hpp>
#include <Basix/Profiling/CpuTimer.hpp>

namespace basix
{

auto CpuTimeTaskProfiler::startTimingNewTask(
    std::string_view const name,
    std::string_view const description)
    -> ScopedCpuTimeProfiling
{
    auto metrics = std::make_unique<CpuTimeMetrics>();

    auto & m = *metrics;

    auto task = ProfiledTask{name, description, std::move(metrics)};

    auto closer = ProfiledTaskBuilder::openNewTask(std::move(task));

    auto timer = CpuTimer{m};

    return {std::move(closer), std::move(timer)};
}

auto CpuTimeTaskProfiler::getProfiledTask()
    -> ProfiledTask &
{
    return ProfiledTaskBuilder::getRootProfiledTask();
}

} // namespace basix
