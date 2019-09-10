#pragma once

#include <GLFW/glfw3.h>

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
