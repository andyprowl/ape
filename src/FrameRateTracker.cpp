#include "FrameRateTracker.hpp"

#include "FrameTimeTracker.hpp"

#include "GLFW.hpp"

#include <iostream>

FrameRateTracker::FrameRateTracker(
    FrameTimeTracker & timeTracker,
    int const reportFrequencyInMs)
    : timeTracker{&timeTracker}
    , reportFrequencyInMs{reportFrequencyInMs}
    , numOfReports{0}
    , numOfSamplesInBurst{0}
    , burstDuration{0.0}
    , isStopped{true}
{
}

auto FrameRateTracker::start()
    -> void
{
    isStopped = false;
}

auto FrameRateTracker::stop()
    -> void
{
    isStopped = true;
}

auto FrameRateTracker::update()
    -> void
{
    burstDuration += timeTracker->getLastFrameDuration();

    ++numOfSamplesInBurst;

    if (isTimeToReport())
    {
        report();

        numOfSamplesInBurst = 0;

        burstDuration = 0.0;

        ++numOfReports;
    }
}

auto FrameRateTracker::isTimeToReport() const
    -> bool
{
    auto const elapsedMilliseconds = static_cast<int>(glfwGetTime() * 1000);

    return ((elapsedMilliseconds / reportFrequencyInMs) > numOfReports);
}

auto FrameRateTracker::report() const
    -> void
{
    auto const framesPerSecond = (numOfSamplesInBurst / burstDuration);

    std::cout << "FPS: " << framesPerSecond << std::endl;
}
