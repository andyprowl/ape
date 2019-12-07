#pragma once

#include <Basix/Profiling/ProfiledTask.hpp>
#include <Basix/Profiling/ScopedTaskCloser.hpp>

#include <cassert>
#include <memory>

namespace basix
{

class ProfiledTaskBuilder
{

public:

    ProfiledTaskBuilder()
        : rootTask{nullptr}
        , currentTask{nullptr}
    {
    }

    auto openNewTask(ProfiledTask task)
        -> ScopedTaskCloser
    {
        if (currentTask == nullptr)
        {
            rootTask = std::make_unique<ProfiledTask>(std::move(task));

            currentTask = rootTask.get();
        }
        else
        {
            currentTask = &(currentTask->addSubTask(std::move(task)));
        }

        return ScopedTaskCloser{*this};
    }

    auto closeCurrentTask()
        -> void
    {
        assert(currentTask != nullptr);

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

    std::unique_ptr<ProfiledTask> rootTask;

    ProfiledTask * currentTask;

};

} // namespace basix
