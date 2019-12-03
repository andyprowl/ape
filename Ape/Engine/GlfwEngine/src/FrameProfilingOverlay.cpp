#include "FrameProfilingOverlay.hpp"

#include "ImGuiWindow.hpp"

#include <Ape/Rendering/Windowing/Window.hpp>

#include <Basix/Range/Search.hpp>

#include <DearImGui/imgui.h>

#include <algorithm>

namespace ape
{

namespace
{

auto frameProfileGetter(void * const data, int const index)
    -> float
{
    using FrameProfileBuffer = FrameProfilingOverlay::FrameProfileBuffer;

    auto const & profileBuffer = *(reinterpret_cast<FrameProfileBuffer const *>(data));

    auto const & profile = profileBuffer[index];

    auto const durationInMicroseconds = profile.getDuration().count();

    return durationInMicroseconds / 1'000.f;
}

} // unnamed namespace

FrameProfilingOverlay::FrameProfilingOverlay(
    basix::Position<int> const & initialPosition,
    basix::Size<int> const & initialSize,
    FrameProfileBuffer const & frameProfileBuffer)
    : initialPosition{initialPosition}
    , initialSize{initialSize}
    , frameProfileBuffer{&frameProfileBuffer}
    , doNotRecordFrameProfiles{false}
    , maxNumOfPlottedFrames{frameProfileBuffer.capacity() / 2}
    , frameDurationCapInMs{50}
    , selectedFrameIndex{-1}
{
}

auto FrameProfilingOverlay::update()
    -> void
{
    auto const window = makeWindow();

    updatePauseProfiling();

    updateMaxNumOfPlottedFrames();

    updateFrameDurationCap();

    updateFrameProfileHistogram();
}

auto FrameProfilingOverlay::isFrameProfilingPaused() const
    -> bool
{
    return doNotRecordFrameProfiles;
}

auto FrameProfilingOverlay::getSelectedFrameProfile() const
    -> basix::TaskProfile const *
{
    if (selectedFrameIndex < 0)
    {
        return nullptr;
    }

    auto const & selectedProfile = (*frameProfileBuffer)[selectedFrameIndex];

    return &selectedProfile;
}

auto FrameProfilingOverlay::makeWindow() const
    -> ImGuiWindow
{
    return ImGuiWindow{"FrameProfiling", initialPosition, initialSize};
}

auto FrameProfilingOverlay::updatePauseProfiling()
    -> void
{
    ImGui::Checkbox("Pause frame profiling", &doNotRecordFrameProfiles);

    if (!doNotRecordFrameProfiles)
    {
        selectedFrameIndex = -1;
    }
}

auto FrameProfilingOverlay::updateMaxNumOfPlottedFrames()
    -> void
{
    auto const profileBufferCapacity = frameProfileBuffer->capacity();

    ImGui::SliderInt(
        "Maximum number of plotted frames",
        &maxNumOfPlottedFrames,
        0,
        profileBufferCapacity);
}

auto FrameProfilingOverlay::updateFrameDurationCap()
    -> void
{
    ImGui::SliderInt("Frame duration cap (in ms)", &frameDurationCapInMs, 1, 100);
}

auto FrameProfilingOverlay::updateFrameProfileHistogram()
    -> void
{
    if (!doNotRecordFrameProfiles)
    {
    }

    auto const numOfFramesToPlot = std::min(maxNumOfPlottedFrames, frameProfileBuffer->size());

    auto const availableWindowSpace = ImGui::GetContentRegionAvail();

    ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, {1.0f, 0.3f, 1.0f, 1.0f});

    ImGui::PlotHistogram(
        "Frame time histogram",
        frameProfileGetter,
        const_cast<void *>(static_cast<void const *>(frameProfileBuffer)),
        numOfFramesToPlot,
        frameProfileBuffer->size() - numOfFramesToPlot,
        nullptr,
        0.0f,
        static_cast<float>(frameDurationCapInMs),
        {availableWindowSpace.x, availableWindowSpace.y},
        nullptr,
        &selectedFrameIndex);

    ImGui::PopStyleColor();

    if ((selectedFrameIndex >= 0) && ImGui::IsItemClicked())
    {
        doNotRecordFrameProfiles = true;
    }
}

} // namespace ape
