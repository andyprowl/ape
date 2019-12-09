#pragma once

#include <Basix/Profiling/TaskProfileMetrics.hpp>

#include <memory>
#include <string_view>
#include <vector>

namespace basix
{

class ProfiledTask
{

public:

    using SubTaskContainer = std::vector<ProfiledTask>;

    using MetricsContainer = std::vector<std::unique_ptr<TaskProfileMetrics>>;

public:

    ProfiledTask(
        std::string_view name,
        std::string_view description = "",
        MetricsContainer newMetrics = {});

    ProfiledTask(ProfiledTask const & rhs);

    ProfiledTask(ProfiledTask && rhs) noexcept;

    auto operator = (ProfiledTask const & rhs)
        -> ProfiledTask &;

    auto operator = (ProfiledTask && rhs) noexcept
        -> ProfiledTask &;

    ~ProfiledTask() = default;

    auto getName() const
        -> std::string_view;

    auto getDescription() const
        -> std::string_view;

    auto getParent() const
        -> ProfiledTask *;

    auto getSubTasks() const
        -> SubTaskContainer const &;

    auto addSubTask(ProfiledTask task)
        -> ProfiledTask &;

    auto getMetrics() const
        -> MetricsContainer const &;

    auto setMetrics(MetricsContainer newMetrics)
        -> void;

private:

    auto setSelfAsParentOfSubTasks()
        -> void;

    auto cloneMetrics() const
        -> MetricsContainer;

private:

    std::string_view name;

    std::string_view description;

    ProfiledTask * parent;

    SubTaskContainer subTasks;

    MetricsContainer metrics;

};

} // namespace basix
