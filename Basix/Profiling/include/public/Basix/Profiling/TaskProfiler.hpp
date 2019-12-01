#pragma once

#include <Basix/Profiling/TaskProfile.hpp>
#include <Basix/Profiling/ScopedTaskProfiling.hpp>

#include <cassert>
#include <memory>
#include <string_view>
#include <type_traits>

namespace basix
{

class TaskProfiler
{

public:

    TaskProfiler()
        : rootProfile{nullptr}
        , activeProfile{nullptr}
    {
    }

    auto startProfilingTask(std::string_view const name, std::string_view const description)
        -> ScopedTaskProfiling
    {
        return startProfilingTask<TaskProfile>(name, description);
    }

    template<typename ProfileType>
    auto startProfilingTask(std::string_view const name, std::string_view const description)
        -> ScopedTaskProfiling
    {
        static_assert(
            std::is_base_of_v<TaskProfile, ProfileType>,
            "ProfileType must derive from TaskProfile!");

        auto profile = std::make_unique<ProfileType>(name, description, ProfileType::Clock::now());

        if (rootProfile == nullptr)
        {
            rootProfile = std::move(profile);

            activeProfile = rootProfile.get();
        }
        else
        {
            auto & newSubProfile = activeProfile->addSubProfile(std::move(profile));

            activeProfile = &newSubProfile;
        }

        return {*this, *activeProfile};
    }

    auto recordActiveTaskCompletion()
        -> void
    {
        assert(activeProfile != nullptr);

        recordTaskCompletion(*activeProfile);

        activeProfile = activeProfile->getParent();
    }

    auto getCurrentTaskProfile()
        -> TaskProfile &
    {
        return *activeProfile;
    }

    auto getRootTaskProfile()
        -> TaskProfile &
    {
        return *rootProfile;
    }

private:

    std::unique_ptr<TaskProfile> rootProfile;

    TaskProfile * activeProfile;

};

} // namespace basix
