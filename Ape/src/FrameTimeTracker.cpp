#include <Ape/FrameTimeTracker.hpp>

#include "GLFW.hpp"

namespace ape
{

FrameTimeTracker::FrameTimeTracker()
    : lastTime{glfwGetTime()}
    , lastFrameDuration{0.0}
{
}

auto FrameTimeTracker::update()
    -> void
{
    auto const currentTime = glfwGetTime();

    lastFrameDuration = currentTime - lastTime;

    lastTime = currentTime;
}

auto FrameTimeTracker::getLastFrameDuration() const
    -> double
{
    return lastFrameDuration;
}

} // namespace ape
