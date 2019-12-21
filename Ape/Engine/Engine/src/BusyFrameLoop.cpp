#include <Ape/Engine/Engine/BusyFrameLoop.hpp>

namespace ape
{

BusyFrameLoop::BusyFrameLoop()
    : stopBeforeNextIteration{false}
{
}

// virtual (from FrameLoop)
auto BusyFrameLoop::start(FrameProcessor const processor)
    -> void
{
    stopBeforeNextIteration = false;

    while (!wasStopRequested())
    {
        processor();
    }
}

// virtual (from FrameLoop)
auto BusyFrameLoop::stop()
    -> void
{
    stopBeforeNextIteration = true;
}

auto BusyFrameLoop::wasStopRequested() const
    -> bool
{
    return stopBeforeNextIteration;
}

} // namespace ape
