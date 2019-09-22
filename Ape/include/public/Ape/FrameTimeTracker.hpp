#pragma once

namespace ape
{

class FrameTimeTracker
{

public:

    FrameTimeTracker();

    auto update()
        -> void;

    auto getLastFrameDuration() const
        -> double;

private:

    double lastTime;

    double lastFrameDuration;

};

} // namespace ape
