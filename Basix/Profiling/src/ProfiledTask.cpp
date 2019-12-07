#pragma once

#include <Basix/Profiling/ProfiledTask.hpp>

namespace basix
{

ProfiledTask::ProfiledTask(
    std::string_view const name,
    std::string_view const description,
    std::unique_ptr<TaskProfileMetrics> metrics)
    : name{name}
    , description{description}
    , parent{nullptr}
    , subTasks{}
    , metrics{std::move(metrics)}
{
}

ProfiledTask::ProfiledTask(ProfiledTask const & rhs)
    : name{rhs.name}
    , description{rhs.description}
    , parent{rhs.parent}
    , subTasks{rhs.subTasks}
    , metrics{rhs.metrics->clone()}
{
    setSelfAsParentOfSubTasks();
}

ProfiledTask::ProfiledTask(ProfiledTask && rhs) noexcept
    : name{rhs.name}
    , description{rhs.description}
    , parent{std::move(rhs.parent)}
    , subTasks{std::move(rhs.subTasks)}
    , metrics{std::move(rhs.metrics)}
{
    setSelfAsParentOfSubTasks();
}

auto ProfiledTask::operator = (ProfiledTask const & rhs)
    -> ProfiledTask &
{
    name = rhs.name;
    
    description = rhs.description;
    
    parent = rhs.parent;
    
    subTasks = rhs.subTasks;
    
    metrics = rhs.metrics->clone();

    setSelfAsParentOfSubTasks();

    return *this;
}

auto ProfiledTask::operator = (ProfiledTask && rhs) noexcept
    -> ProfiledTask &
{
    name = rhs.name;
    
    description = rhs.description;
    
    parent = std::move(rhs.parent);
    
    subTasks = std::move(rhs.subTasks);
    
    metrics = std::move(rhs.metrics);

    setSelfAsParentOfSubTasks();

    return *this;
}

auto ProfiledTask::getName() const
    -> std::string_view
{
    return name;
}

auto ProfiledTask::getDescription() const
    -> std::string_view
{
    return description;
}

auto ProfiledTask::getParent() const
    -> ProfiledTask * 
{
    return parent;
}

auto ProfiledTask::getSubTasks() const
    -> SubTaskContainer const &
{
    return subTasks;
}

auto ProfiledTask::addSubTask(ProfiledTask task)
    -> ProfiledTask &
{
    task.parent = this;

    subTasks.push_back(std::move(task));

    return subTasks.back();
}

auto ProfiledTask::getMetrics() const
    -> TaskProfileMetrics &
{
    return *metrics;
}

auto ProfiledTask::setMetrics(std::unique_ptr<TaskProfileMetrics> newMetrics)
    -> void
{
    metrics = std::move(newMetrics);
}

auto ProfiledTask::setSelfAsParentOfSubTasks()
    -> void
{
    for (auto & subTask : subTasks)
    {
        subTask.parent = this;
    }
}

} // namespace basix
