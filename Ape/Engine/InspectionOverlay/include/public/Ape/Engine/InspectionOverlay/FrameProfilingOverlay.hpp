#pragma once

#include <Basix/Container/CircularBuffer.hpp>
#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>
#include <Basix/Profiling/ProfiledTask.hpp>

namespace ape
{

class ImGuiWindow;
class Window;

class FrameProfilingOverlay
{

public:

    using FrameProfileBuffer = basix::CircularBuffer<basix::ProfiledTask>;

public:

    FrameProfilingOverlay(
        basix::Position<int> const & initialPosition,
        basix::Size<int> const & initialSize,
        FrameProfileBuffer const & frameProfileBuffer);

    auto update()
        -> void;

    auto isFrameProfilingPaused() const
        -> bool;

    auto getSelectedFrameProfile() const
        -> basix::ProfiledTask const *;

private:

    auto makeWindow() const
        -> ImGuiWindow;

    auto updatePauseProfiling()
        -> void;

    auto updateMaxNumOfPlottedFrames()
        -> void;

    auto updateFrameDurationCap()
        -> void;

    auto updateFrameProfileHistogram()
        -> void;

    auto updateFrameProfileDetails()
        -> void;

    auto updateFrameProcessingSubTask(basix::ProfiledTask const & profile)
        -> int;

    auto plotFrameProfileHistogram(int numOfFramesToPlot, float histogramWidth)
        -> void;

private:

    basix::Position<int> initialPosition;

    basix::Size<int> initialSize;

    FrameProfileBuffer const * frameProfileBuffer;

    float lastHistogramHeight;

    float lastWindowHeight;

    bool isProfilingPaused;

    int maxNumOfPlottedFrames;

    int frameDurationCapInMs;

    int selectedFrameIndex;

};

} // namespace ape
