#pragma once

#include <chrono>

namespace basix
{

class TaskTiming
{

public:

    using Clock = std::chrono::high_resolution_clock;

    using TimePoint = Clock::time_point;

    using Microseconds = std::chrono::microseconds;

public:

    TaskTiming() = default;

    TaskTiming(TimePoint const startTime, Microseconds const duration)
        : startTime{startTime}
        , duration{duration}
    {
    }

    auto setStartTime(TimePoint const newStartTime)
        -> void
    {
        startTime = newStartTime;
    }

    auto setEndTime(TimePoint const endTime)
        -> void
    {
        duration = std::chrono::duration_cast<Microseconds>(endTime - startTime);
    }

    auto getDuration() const
        -> Microseconds
    {
        return duration;
    }

private:

    TimePoint startTime;

    Microseconds duration;

};

} // namespace basix
