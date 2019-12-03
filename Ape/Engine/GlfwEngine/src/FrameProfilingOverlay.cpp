#include "FrameProfilingOverlay.hpp"

#include "ImGuiWindow.hpp"

#include <Ape/Rendering/Windowing/Window.hpp>

#include <DearImGui/imgui.h>

#include <algorithm>
#include <cassert>

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
    , lastHistogramHeight{0.0f}
    , lastWindowHeight{0.0f}
    , isProfilingPaused{false}
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

    updateFrameProfileDetails();
}

auto FrameProfilingOverlay::isFrameProfilingPaused() const
    -> bool
{
    return isProfilingPaused;
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
    return ImGuiWindow{"Frame Profiling", initialPosition, initialSize};
}

auto FrameProfilingOverlay::updatePauseProfiling()
    -> void
{
    auto const wasProfilingPaused = isProfilingPaused;

    ImGui::Checkbox("Pause frame profiling", &isProfilingPaused);

    if (wasProfilingPaused && !isProfilingPaused)
    {
        selectedFrameIndex = -1;

        ImGui::SetWindowSize({0.0f, lastWindowHeight});
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
    auto const numOfFramesToPlot = std::min(maxNumOfPlottedFrames, frameProfileBuffer->size());

    auto availableWindowSpace = ImGui::GetContentRegionAvail();
    
    if (selectedFrameIndex < 0)
    {
        lastHistogramHeight = availableWindowSpace.y - ImGui::GetFrameHeightWithSpacing();

        lastWindowHeight = ImGui::GetWindowHeight();
    }

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
        {availableWindowSpace.x, lastHistogramHeight},
        nullptr,
        &selectedFrameIndex);

    ImGui::PopStyleColor();

    if ((selectedFrameIndex >= 0) && ImGui::IsItemClicked())
    {
        isProfilingPaused = true;
    }
}

auto FrameProfilingOverlay::updateFrameProfileDetails()
    -> void
{
    static auto resize = true;

    if (selectedFrameIndex < 0)
    {
        ImGui::Text("No frame selected");

        resize = true;

        return;
    }
    
    auto const selectedProfile = getSelectedFrameProfile();

    assert(selectedProfile != nullptr);

    auto const numOfTreeItems = updateFrameProcessingTaskProfile(*selectedProfile);

    if (resize)
    {
        auto const itemHeight = ImGui::GetItemRectSize().y;

        ImGui::SetWindowSize({0.0f, lastWindowHeight + numOfTreeItems * itemHeight});

        resize = false;
    }
}

auto FrameProfilingOverlay::updateFrameProcessingTaskProfile(basix::TaskProfile const & profile)
    -> int
{
    ImGui::Columns(2, "Timings", true);

    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    auto const isExpanded = ImGui::TreeNode(profile.getName().data());

    ImGui::NextColumn();

    ImGui::Text("%f ms", profile.getDuration().count() / 1'000.f);

    ImGui::NextColumn();

    if (!isExpanded)
    {
        return 1;
    }

    auto numOfItems = 1;

    for (auto const & subProfile : profile.getSubProfiles())
    {
        numOfItems += updateFrameProcessingTaskProfile(*subProfile);
    }

    ImGui::TreePop();

    return numOfItems;
}

} // namespace ape
