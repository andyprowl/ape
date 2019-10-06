#include <Time/TimeIntervalTracker.hpp>

#include <Time/Stopwatch.hpp>

namespace ape
{

TimeIntervalTracker::TimeIntervalTracker(Stopwatch const & stopwatch)
    : stopwatch{&stopwatch}
    , lastElapsedTime{stopwatch.getElapsedTime()}
    , lastFrameDuration{std::chrono::nanoseconds{0}}
{
}

auto TimeIntervalTracker::tick()
    -> std::chrono::nanoseconds
{
    auto const timeFromStart = stopwatch->getElapsedTime();

    lastFrameDuration = timeFromStart - lastElapsedTime;

    lastElapsedTime = timeFromStart;

    return lastFrameDuration;
}

auto TimeIntervalTracker::getLastIntervalDuration() const
    -> std::chrono::nanoseconds
{
    return lastFrameDuration;
}

auto TimeIntervalTracker::getStopwatch() const
    -> Stopwatch const &
{
    return *stopwatch;
}

} // namespace ape
