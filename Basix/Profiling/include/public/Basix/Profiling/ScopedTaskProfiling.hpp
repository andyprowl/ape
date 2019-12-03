#pragma once

#include <memory>

namespace basix
{

class TaskProfile;
class TaskProfiler;

class [[nodiscard]] ScopedTaskProfiling
{

public:

    ScopedTaskProfiling()
        : completer{nullptr}
        , profile{nullptr}
    {
    }

    ScopedTaskProfiling(TaskProfiler & profiler, TaskProfile & profile)
        : completer{&profiler}
        , profile{&profile}
    {
    }

    auto getProfile() const
        -> TaskProfile &
    {
        return *profile;
    }

    auto detach()
        -> void
    {
        completer.release();
    }

private:

    class ProfileCompleter
    {
    
    public:

        auto operator () (TaskProfiler * profiler) const
            -> void;
    
    };

private:

    std::unique_ptr<TaskProfiler, ProfileCompleter> completer;

    TaskProfile * profile;

};

} // namespace basix
