#pragma once

#include <chrono>

namespace ape
{

class Stopwatch;

class FrameTimeTracker
{

public:

    explicit FrameTimeTracker(Stopwatch const & stopwatch);

    auto update()
        -> void;

    auto getLastFrameDuration() const
        -> std::chrono::nanoseconds;

    auto getStopwatch() const
        -> Stopwatch const &;

private:

    Stopwatch const * stopwatch;

    std::chrono::nanoseconds lastElapsedTime;

    std::chrono::nanoseconds lastFrameDuration;

};

} // namespace ape
