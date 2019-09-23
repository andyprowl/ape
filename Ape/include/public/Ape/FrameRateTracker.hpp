#pragma once

#include <chrono>

namespace ape
{

class FrameTimeTracker;

class FrameRateTracker
{

public:

    FrameRateTracker(FrameTimeTracker const & timeTracker, int reportFrequencyInMs);

    auto start()
        -> void;

    auto stop()
        -> void;

    auto update()
        -> void;

private:

    auto isTimeToReport() const
        -> bool;

    auto report() const
        -> void;

private:

    FrameTimeTracker const * timeTracker;

    int reportFrequencyInMs;

    int numOfReports;

    int numOfSamplesInBurst;

    std::chrono::nanoseconds burstDuration;

    bool isStopped;

};

} // namespace ape
