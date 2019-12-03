#pragma once

#include <chrono>
#include <memory>
#include <string_view>
#include <vector>

namespace basix
{

class TaskProfile
{

public:

    using SubProfileContainer = std::vector<std::unique_ptr<TaskProfile>>;

    using Clock = std::chrono::high_resolution_clock;

    using TimePoint = Clock::time_point;

    using Microseconds = std::chrono::microseconds;

public:

    TaskProfile(
        std::string_view name,
        std::string_view description,
        TimePoint startTime = TimePoint{},
        Microseconds duration = {});

    auto getName() const
        -> std::string_view;

    auto getDescription() const
        -> std::string_view;

    auto getStartTime() const
        -> TimePoint;

    auto getEndTime() const
        -> TimePoint;

    auto setEndTime(TimePoint endTime)
        -> void;

    auto getDuration() const
        -> Microseconds;

    auto getParent() const
        -> TaskProfile *;

    auto getSubProfiles() const
        -> SubProfileContainer const &;

    auto addSubProfile(std::unique_ptr<TaskProfile> profile)
        -> TaskProfile &;

private:

    std::string_view name;

    std::string_view description;

    TimePoint startTime;

    Microseconds duration;

    TaskProfile * parent;

    SubProfileContainer subProfiles;

};

inline auto recordTaskCompletion(TaskProfile & profile)
    -> void
{
    profile.setEndTime(TaskProfile::Clock::now());
}

} // namespace basix