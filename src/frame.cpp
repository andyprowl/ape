#include "frame.hpp"

FrameTimeTracker::FrameTimeTracker()
    : lastTime{glfwGetTime()}
    , lastFrameDuration{0.0}
{
}

auto FrameTimeTracker::update()
    -> void
{
    const auto currentTime = glfwGetTime();

    lastFrameDuration = currentTime - lastTime;

    lastTime = currentTime;
}

auto FrameTimeTracker::getLastFrameDuration() const
    -> double
{
    return lastFrameDuration;
}
