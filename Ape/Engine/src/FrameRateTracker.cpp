#include <Engine/FrameRateTracker.hpp>

#include <Engine/Stopwatch.hpp>
#include <Engine/TimeIntervalTracker.hpp>

#include <iostream>

namespace ape
{

FrameRateTracker::FrameRateTracker(
    TimeIntervalTracker const & timeTracker,
    int const reportFrequencyInMs)
    : timeTracker{&timeTracker}
    , reportFrequencyInMs{reportFrequencyInMs}
    , numOfReports{0}
    , numOfSamplesInBurst{0}
    , burstDuration{std::chrono::nanoseconds{0}}
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

    numOfSamplesInBurst = 0;

    burstDuration = std::chrono::nanoseconds{0};
}

auto FrameRateTracker::update()
    -> void
{
    if (isStopped)
    {
        return;
    }

    burstDuration += timeTracker->getLastIntervalDuration();

    ++numOfSamplesInBurst;

    if (isTimeToReport())
    {
        report();

        numOfSamplesInBurst = 0;

        burstDuration = std::chrono::nanoseconds{0};

        ++numOfReports;
    }
}

auto FrameRateTracker::isTimeToReport() const
    -> bool
{
    auto const timeSinceStart = timeTracker->getStopwatch().getElapsedTime();

    auto const elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceStart);

    return ((elapsedMs.count() / reportFrequencyInMs) > numOfReports);
}

auto FrameRateTracker::report() const
    -> void
{
    auto const burstDurationInSeconds = (burstDuration.count() / 1'000'000'000.0);

    auto const framesPerSecond = (numOfSamplesInBurst / burstDurationInSeconds);

    std::cout << "FPS: " << framesPerSecond << std::endl;
}

} // namespace ape
