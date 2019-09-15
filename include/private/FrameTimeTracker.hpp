#pragma once

#include "GLFW.hpp"

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
