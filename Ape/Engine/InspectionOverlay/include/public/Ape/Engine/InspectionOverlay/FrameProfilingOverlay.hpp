#pragma once

#include <Basix/Container/CircularBuffer.hpp>
#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>
#include <Basix/Profiling/ProfiledTask.hpp>

namespace ape
{

class ImGuiWindow;
class TaskTimeProfiler;
class Window;

class FrameProfilingOverlay
{

public:

    using FrameProfileBuffer = basix::CircularBuffer<basix::ProfiledTask>;

public:

    FrameProfilingOverlay(
        basix::Position<int> const & initialPosition,
        basix::Size<int> const & initialSize,
        TaskTimeProfiler & frameProfiler,
        FrameProfileBuffer const & frameProfileBuffer);

    auto update()
        -> void;

    auto getSelectedFrameProfile() const
        -> basix::ProfiledTask const *;

private:

    auto makeWindow() const
        -> ImGuiWindow;

    auto updateProfilingOptions()
        -> void;

    auto updatePauseProfiling()
        -> void;

    auto updateCollectGpuTimeMetrics()
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

    auto updateFrameProcessingSubTaskOwnMetrics(basix::ProfiledTask const & profile)
        -> bool;

    auto updateCpuTimeMetrics(basix::ProfiledTask const & profile)
        -> void;

    auto updateGpuTimeMetrics(basix::ProfiledTask const & profile)
        -> void;

    auto plotFrameProfileHistogram(int numOfFramesToPlot, float histogramWidth)
        -> void;

private:

    basix::Position<int> initialPosition;

    basix::Size<int> initialSize;

    TaskTimeProfiler * frameProfiler;

    FrameProfileBuffer const * frameProfileBuffer;

    float lastHistogramHeight;

    float lastWindowHeight;

    bool isProfilingPaused;

    bool collectGpuTimeMetrics;

    int maxNumOfPlottedFrames;

    int frameDurationCapInMs;

    int selectedFrameIndex;

};

} // namespace ape
