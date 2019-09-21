#pragma once

class FrameTimeTracker;

class FrameRateTracker
{

public:

    FrameRateTracker(FrameTimeTracker & timeTracker, int reportFrequencyInMs);

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

    FrameTimeTracker * timeTracker;

    int reportFrequencyInMs;

    int numOfReports;

    int numOfSamplesInBurst;

    double burstDuration;

    bool isStopped;

};
