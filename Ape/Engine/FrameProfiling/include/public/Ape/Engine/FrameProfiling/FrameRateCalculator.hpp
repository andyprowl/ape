#pragma once

#include <Basix/Container/CircularBuffer.hpp>
#include <Basix/Profiling/CpuTimeMetrics.hpp>
#include <Basix/Profiling/ProfiledTask.hpp>

namespace ape
{

class FrameRateCalculator
{

public:

    using FrameProfileBuffer = basix::CircularBuffer<basix::ProfiledTask>;

public:

    FrameRateCalculator(FrameProfileBuffer const & profiles, int maxNumOfSamples);

    // This function must be invoked every time a new frame profile is added to the frame profile
    // buffer in order for calculations to give correct results.
    auto update()
        -> void;

    auto getAverageFrameDurationInMs() const
        -> float;

    auto getAverageFrameRate() const
        -> float;

private:

    FrameProfileBuffer const * profiles;

    int maxNumOfSamples;

    int numOfSamples;

    basix::CpuTimeMetrics::Nanoseconds cumulativeDuration;

};

} // namespace ape
