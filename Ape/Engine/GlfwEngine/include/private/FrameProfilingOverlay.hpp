#pragma once

#include <Basix/Container/CircularBuffer.hpp>
#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>
#include <Basix/Profiling/TaskProfile.hpp>

namespace ape
{

class ImGuiWindow;
class Window;

class FrameProfilingOverlay
{

public:

    using FrameProfileBuffer = basix::CircularBuffer<basix::TaskProfile>;

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
        -> basix::TaskProfile const *;

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

private:

    basix::Position<int> initialPosition;

    basix::Size<int> initialSize;

    FrameProfileBuffer const * frameProfileBuffer;

    bool doNotRecordFrameProfiles;

    int maxNumOfPlottedFrames;

    int frameDurationCapInMs;

    int selectedFrameIndex;

};

} // namespace ape
