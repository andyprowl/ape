#pragma once

#include <chrono>

namespace ape
{

class TimeIntervalTracker;

class FrequencyTracker
{

public:

    FrequencyTracker(TimeIntervalTracker const & timeTracker, int reportFrequencyInMs);

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

    TimeIntervalTracker const * timeTracker;

    int reportFrequencyInMs;

    int numOfReports;

    int numOfSamplesInBurst;

    std::chrono::nanoseconds burstDuration;

    bool isStopped;

};

} // namespace ape
