#pragma once

#include <Basix/Profiling/ProfiledTask.hpp>
#include <Basix/Profiling/ScopedTaskProfiling.hpp>

#include <Basix/Meta/Swallow.hpp>

#include <cassert>
#include <memory>
#include <optional>

namespace basix
{

class TaskProfiler
{

public:

    TaskProfiler()
        : rootTask{std::nullopt}
        , currentTask{nullptr}
    {
    }

    template<typename... MetricsCollectors>
    auto openNewTaskProfile(
        std::string_view const name,
        std::string_view const description,
        MetricsCollectors... collectors)
        -> ScopedTaskProfiling<MetricsCollectors...>
    {
        if (currentTask == nullptr)
        {
            rootTask = ProfiledTask{name, description};

            currentTask = &(*rootTask);
        }
        else
        {
            currentTask = &(currentTask->addSubTask({name, description}));
        }

        return {*this, std::make_tuple(std::move(collectors)...)};
    }

    template<typename... MetricsCollectors>
    auto closeCurrentTask(std::tuple<MetricsCollectors...> collectors)
        -> void
    {
        assert(currentTask != nullptr);

        auto metrics = extractMetrics(std::move(collectors));

        currentTask->setMetrics(std::move(metrics));

        currentTask = currentTask->getParent();
    }

    auto getCurrentProfiledTask()
        -> ProfiledTask &
    {
        return *currentTask;
    }

    auto getRootProfiledTask()
        -> ProfiledTask &
    {
        return *rootTask;
    }

private:

    template<typename... MetricsCollectors>
    auto extractMetrics(std::tuple<MetricsCollectors...> collectors) const
        -> ProfiledTask::MetricsContainer
    {
        auto const indices = std::make_index_sequence<sizeof...(MetricsCollectors)>{};

        return extractMetrics(collectors, indices);
    }

    template<typename... MetricsCollectors, std::size_t... Is>
    auto extractMetrics(
        std::tuple<MetricsCollectors...> & collectors,
        std::index_sequence<Is...>) const
        -> ProfiledTask::MetricsContainer
    {
        auto metricsSet = ProfiledTask::MetricsContainer{};

        Swallow{(metricsSet.push_back(extractMetrics(std::get<Is>(collectors))), 0)...};

        return metricsSet;
    }

    template<typename MetricsCollector>
    auto extractMetrics(MetricsCollector & collector) const
        -> std::unique_ptr<TaskProfileMetrics>
    {
        return collector.extractMetrics();
    }

    template<typename MetricsCollector>
    auto extractMetrics(std::optional<MetricsCollector> & collector) const
        -> std::unique_ptr<TaskProfileMetrics>
    {
        if (not collector.has_value())
        {
            return nullptr;
        }

        return collector->extractMetrics();
    }

private:

    std::optional<ProfiledTask> rootTask;

    ProfiledTask * currentTask;

};

} // namespace basix
