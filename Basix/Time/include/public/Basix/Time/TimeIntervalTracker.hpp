#pragma once

#include <chrono>

namespace basix
{

class Stopwatch;

class TimeIntervalTracker
{

public:

    explicit TimeIntervalTracker(Stopwatch const & stopwatch);

    auto tick()
        -> std::chrono::nanoseconds;

    auto getLastIntervalDuration() const
        -> std::chrono::nanoseconds;

    auto getStopwatch() const
        -> Stopwatch const &;

private:

    Stopwatch const * stopwatch;

    std::chrono::nanoseconds lastElapsedTime;

    std::chrono::nanoseconds lastFrameDuration;

};

} // namespace basix
