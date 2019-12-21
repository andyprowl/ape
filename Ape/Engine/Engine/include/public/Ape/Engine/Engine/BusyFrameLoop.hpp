#pragma once

#include <Ape/Engine/Engine/FrameLoop.hpp>

namespace ape
{

class BusyFrameLoop : public FrameLoop
{

public:

    BusyFrameLoop();

    // virtual (from FrameLoop)
    auto start(FrameProcessor processor)
        -> void override;

    // virtual (from FrameLoop)
    auto stop()
        -> void override;

private:

    auto wasStopRequested() const
        -> bool;

private:

    bool stopBeforeNextIteration;

};

} // namespace ape
