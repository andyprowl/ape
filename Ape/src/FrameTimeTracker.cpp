#include <Ape/FrameTimeTracker.hpp>

#include <Ape/Stopwatch.hpp>

namespace ape
{

FrameTimeTracker::FrameTimeTracker(Stopwatch const & stopwatch)
    : stopwatch{&stopwatch}
    , lastElapsedTime{stopwatch.getElapsedTime()}
    , lastFrameDuration{std::chrono::nanoseconds{0}}
{
}

auto FrameTimeTracker::update()
    -> void
{
    auto const timeFromStart = stopwatch->getElapsedTime();

    lastFrameDuration = timeFromStart - lastElapsedTime;

    lastElapsedTime = timeFromStart;
}

auto FrameTimeTracker::getLastFrameDuration() const
    -> std::chrono::nanoseconds
{
    return lastFrameDuration;
}

auto FrameTimeTracker::getStopwatch() const
    -> Stopwatch const &
{
    return *stopwatch;
}

} // namespace ape
