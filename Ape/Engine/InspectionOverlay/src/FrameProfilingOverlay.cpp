#include <Ape/Engine/InspectionOverlay/FrameProfilingOverlay.hpp>

#include <Ape/Engine/InspectionOverlay/ImGuiWindow.hpp>

#include <Ape/Rendering/GpuProfiling/GpuTimeMetrics.hpp>

#include <Basix/Profiling/CpuTimeMetrics.hpp>

#include <DearImGui/imgui.h>

#include <algorithm>
#include <cassert>

namespace ape
{

namespace
{

class HistogramSpan
{

public:

    using FrameProfileBuffer = FrameProfilingOverlay::FrameProfileBuffer;

public:

    HistogramSpan(FrameProfileBuffer const & profileBuffer, int const spanSize)
        : profileBuffer{&profileBuffer}
        , baseIndex{profileBuffer.size() - spanSize}
    {
    }

public:

    FrameProfileBuffer const * profileBuffer;

    int baseIndex;

};

auto asMilliseconds(std::chrono::nanoseconds const duration)
    -> float
{
    return duration.count() / 1'000'000.f;
}

auto frameProfileGetter(void * const data, int const index)
    -> float
{
    using FrameProfileBuffer = FrameProfilingOverlay::FrameProfileBuffer;

    auto const & span = *(reinterpret_cast<HistogramSpan *>(data));

    auto const & profile = (*span.profileBuffer)[span.baseIndex + index];

    auto const & metrics = static_cast<basix::CpuTimeMetrics &>(*profile.getMetrics()[0]);

    return asMilliseconds(metrics.duration);
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
    -> basix::ProfiledTask const *
{
    if (selectedFrameIndex < 0)
    {
        return nullptr;
    }

    auto const numOfFramesPlotted = std::min(maxNumOfPlottedFrames, frameProfileBuffer->size());

    auto const firstPlottedFrameIndexInBuffer = frameProfileBuffer->size() - numOfFramesPlotted;

    auto const selectedFrameIndexInBuffer = firstPlottedFrameIndexInBuffer + selectedFrameIndex;

    auto const & selectedProfile = (*frameProfileBuffer)[selectedFrameIndexInBuffer];

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

    plotFrameProfileHistogram(numOfFramesToPlot, availableWindowSpace.x);

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

    auto const numOfTreeItems = updateFrameProcessingSubTask(*selectedProfile);

    if (resize)
    {
        auto const itemHeight = ImGui::GetItemRectSize().y;

        ImGui::SetWindowSize({0.0f, lastWindowHeight + numOfTreeItems * itemHeight});

        resize = false;
    }
}

auto FrameProfilingOverlay::updateFrameProcessingSubTask(basix::ProfiledTask const & profile)
    -> int
{
    ImGui::Columns(3, "Timings", true);

    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    auto const isExpanded = ImGui::TreeNode(profile.getName().data());

    ImGui::NextColumn();

    auto const & cpuMetrics = static_cast<basix::CpuTimeMetrics &>(*profile.getMetrics()[0]);

    ImGui::Text("%f ms", asMilliseconds(cpuMetrics.duration));
    
    ImGui::NextColumn();
    
    auto const & gpuMetrics = static_cast<GpuTimeMetrics &>(*profile.getMetrics()[1]);

    ImGui::Text("%f ms", asMilliseconds(gpuMetrics.getDuration()));

    ImGui::NextColumn();
    
    if (!isExpanded)
    {
        return 1;
    }

    auto numOfItems = 1;

    for (auto const & subTask : profile.getSubTasks())
    {
        numOfItems += updateFrameProcessingSubTask(subTask);
    }

    ImGui::TreePop();

    return numOfItems;
}

auto FrameProfilingOverlay::plotFrameProfileHistogram(
    int const numOfFramesToPlot,
    float const histogramWidth)
    -> void
{
    auto span = HistogramSpan{*frameProfileBuffer, numOfFramesToPlot};

    ImGui::PlotHistogram(
        "Frame time histogram",
        frameProfileGetter,
        static_cast<void *>(&span),
        numOfFramesToPlot,
        0,
        nullptr,
        0.0f,
        static_cast<float>(frameDurationCapInMs),
        {histogramWidth, lastHistogramHeight},
        nullptr,
        &selectedFrameIndex);
}

} // namespace ape
