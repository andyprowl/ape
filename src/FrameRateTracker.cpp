#include "FrameRateTracker.hpp"

#include "FrameTimeTracker.hpp"

#include "GLFW.hpp"

#include <iostream>

FrameRateTracker::FrameRateTracker(FrameTimeTracker & timeTracker, int const reportFrequencyInMs)
    : timeTracker{&timeTracker}
    , reportFrequencyInMs{reportFrequencyInMs}
    , numOfReports{0}
    , numOfSamples{0}
    , burstDuration{0.0}
{
}

auto FrameRateTracker::update()
    -> void
{
    auto const elapsedMilliseconds = static_cast<int>(glfwGetTime() * 1000);
    
    burstDuration += timeTracker->getLastFrameDuration();

    ++numOfSamples;

    if ((elapsedMilliseconds / reportFrequencyInMs) > numOfReports)
    {
        ++numOfReports;

        auto const framesPerSecond = (numOfSamples / burstDuration);

        std::cout << "FPS: " << framesPerSecond << std::endl;

        numOfSamples = 0;

        burstDuration = 0.0;
    }
}
