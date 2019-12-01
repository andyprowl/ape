#pragma once

#include <Basix/Profiling/TaskProfile.hpp>

namespace basix
{

TaskProfile::TaskProfile(
    std::string_view const name,
    std::string_view const description,
    TimePoint const startTime,
    Milliseconds const duration)
    : name{name}
    , description{description}
    , startTime{startTime}
    , duration{duration}
    , parent{nullptr}
    , subProfiles{}
{
}

auto TaskProfile::getName() const
    -> std::string_view
{
    return name;
}

auto TaskProfile::getDescription() const
    -> std::string_view
{
    return description;
}

auto TaskProfile::getStartTime() const
    -> TimePoint
{
    return startTime;
}

auto TaskProfile::getDuration() const
    -> Milliseconds
{
    return duration;
}

auto TaskProfile::getEndTime() const
    -> TimePoint
{
    return startTime + duration;
}

void TaskProfile::setEndTime(TimePoint const endTime)
{
    using namespace std::chrono;

    duration = duration_cast<milliseconds>(endTime - startTime);
}

auto TaskProfile::getParent() const
    -> TaskProfile * 
{
    return parent;
}

auto TaskProfile::getSubProfiles() const
    -> SubProfileContainer const &
{
    return subProfiles;
}

auto TaskProfile::addSubProfile(std::unique_ptr<TaskProfile> profile)
    -> TaskProfile &
{
    auto & subProfile = *profile;

    subProfiles.push_back(std::move(profile));
    
    subProfile.parent = this;

    return subProfile;
}

} // namespace basix
