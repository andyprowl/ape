#pragma once

class FrameTimeTracker;

class FrameRateTracker
{

public:

    explicit FrameRateTracker(FrameTimeTracker & timeTracker, int reportFrequencyInMs);

    auto update()
        -> void;

private:

    FrameTimeTracker * timeTracker;

    int reportFrequencyInMs;

    int numOfReports;

    int numOfSamples;

    double burstDuration;

};
