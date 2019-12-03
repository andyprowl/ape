#pragma once

#include <Basix/Container/CircularBuffer.hpp>
#include <Basix/Profiling/TaskProfile.hpp>

namespace ape
{

class ImGuiWindow;
class Window;

class FrameProfilingOverlay
{

public:

    using FrameProfileBuffer = basix::CircularBuffer<basix::TaskProfile>;
    
    enum class HistogramType
    {
        circular,
        stationary
    };

public:

    FrameProfilingOverlay(Window & parentWindow, FrameProfileBuffer const & lastFrameProfiles);

    auto update()
        -> void;

    auto isFrameProfilingPaused() const
        -> bool;

private:

    auto makeWindow() const
        -> ImGuiWindow;

    auto updatePauseProfiling()
        -> void;

    auto updateHistogramType()
        -> void;

    auto updateMaxNumOfPlottedFrames()
        -> void;

    auto updateFrameDurationCap()
        -> void;

    auto plotFrameProfileHistogram() const
        -> void;

private:

    Window * parentWindow;

    basix::CircularBuffer<basix::TaskProfile> const * lastFrameProfiles;

    bool doNotRecordFrameProfiles;

    HistogramType histogramType;

    int maxNumOfPlottedFrames;

    int frameDurationCapInMs;

};

} // namespace ape
