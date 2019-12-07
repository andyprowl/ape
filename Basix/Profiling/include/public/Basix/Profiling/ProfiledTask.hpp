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

public:

    ProfiledTask(
        std::string_view name,
        std::string_view description,
        std::unique_ptr<TaskProfileMetrics> metrics);

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
        -> TaskProfileMetrics &;

    auto setMetrics(std::unique_ptr<TaskProfileMetrics> newMetrics)
        -> void;

private:

    auto setSelfAsParentOfSubTasks()
        -> void;

private:

    std::string_view name;

    std::string_view description;

    ProfiledTask * parent;

    SubTaskContainer subTasks;

    std::unique_ptr<TaskProfileMetrics> metrics;

};

} // namespace basix
