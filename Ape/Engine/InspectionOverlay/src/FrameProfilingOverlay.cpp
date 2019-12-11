#include <Ape/Engine/InspectionOverlay/FrameProfilingOverlay.hpp>

#include <Ape/Engine/InspectionOverlay/ImGuiWindow.hpp>

#include <Ape/Rendering/GpuProfiling/FrameRateCalculator.hpp>
#include <Ape/Rendering/GpuProfiling/GpuTimeMetrics.hpp>
#include <Ape/Rendering/GpuProfiling/TaskTimeProfiler.hpp>

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
    TaskTimeProfiler & frameProfiler,
    FrameProfileBuffer const & frameProfileBuffer,
    FrameRateCalculator const & frameRateCalculator)
    : initialPosition{initialPosition}
    , initialSize{initialSize}
    , frameProfiler{&frameProfiler}
    , frameProfileBuffer{&frameProfileBuffer}
    , frameRateCalculator{&frameRateCalculator}
    , lastHistogramHeight{0.0f}
    , lastWindowHeight{0.0f}
    , pauseProfiling{false}
    , collectGpuTimeMetrics{true}
    , maxNumOfPlottedFrames{frameProfileBuffer.capacity() / 2}
    , frameDurationCapInMs{50}
    , selectedFrameIndex{-1}
{
}

auto FrameProfilingOverlay::update()
    -> void
{
    auto const window = makeWindow();

    updateAverageFrameRate();

    updateProfilingOptions();

    updateMaxNumOfPlottedFrames();

    updateFrameDurationCap();

    updateFrameProfileHistogram();

    updateFrameProfileDetails();
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

auto FrameProfilingOverlay::isProfilingPaused() const
    -> bool
{
    return pauseProfiling;
}

auto FrameProfilingOverlay::isGpuTimeCollectionEnabled() const
    -> bool
{
    return collectGpuTimeMetrics;
}

auto FrameProfilingOverlay::makeWindow() const
    -> ImGuiWindow
{
    return ImGuiWindow{"Frame Profiling", initialPosition, initialSize};
}

auto FrameProfilingOverlay::updateAverageFrameRate()
    -> void
{
    auto const averageDuration = frameRateCalculator->getAverageFrameDurationInMs();

    if (averageDuration > 0.0f)
    {
        auto const averageFrameRate = 1'000.0 / averageDuration;

        ImGui::Text("Average FPS: %f", averageFrameRate);

        ImGui::SameLine();

        ImGui::Text("\tAverage frame duration: %f", averageDuration);
    }
    else
    {
        ImGui::Text("Average FPS: -");

        ImGui::SameLine();

        ImGui::Text("\tAverage frame duration: -");
    }
}

auto FrameProfilingOverlay::updateProfilingOptions()
    -> void
{
    auto const wasProfilingPaused = isProfilingPaused();
    
    auto const windowWidth = ImGui::GetWindowSize().x;

    ImGui::Checkbox("Pause frame profiling", &pauseProfiling);
    
    ImGui::Checkbox("Collect GPU time metrics", &collectGpuTimeMetrics);

    if (wasProfilingPaused && !isProfilingPaused())
    {
        selectedFrameIndex = -1;

        ImGui::SetWindowSize({windowWidth, lastWindowHeight});
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
        pauseProfiling = true;
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
    auto const isExpanded = updateFrameProcessingSubTaskOwnMetrics(profile);

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

auto FrameProfilingOverlay::updateFrameProcessingSubTaskOwnMetrics(
    basix::ProfiledTask const & profile)
    -> bool
{
    ImGui::Columns(3, "Timings", true);

    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    auto const isExpanded = ImGui::TreeNode(profile.getName().data());

    ImGui::NextColumn();

    updateCpuTimeMetrics(profile);
    
    updateGpuTimeMetrics(profile);

    return isExpanded;
}

auto FrameProfilingOverlay::updateCpuTimeMetrics(basix::ProfiledTask const & profile)
    -> void
{
    auto const & metrics = profile.getMetrics();

    auto const & cpuMetrics = static_cast<basix::CpuTimeMetrics &>(*metrics[0]);

    ImGui::Text("%f ms", asMilliseconds(cpuMetrics.duration));
    
    ImGui::NextColumn();
}

auto FrameProfilingOverlay::updateGpuTimeMetrics(basix::ProfiledTask const & profile)
    -> void
{
    auto const & metrics = profile.getMetrics();

    if ((metrics.size() >= 2u) && (metrics[1] != nullptr))
    {
        auto const & timestampGpuMetrics = static_cast<GpuTimeMetrics &>(*metrics[1]);

        ImGui::Text("%f ms", asMilliseconds(timestampGpuMetrics.duration));
    }
    else
    {
        ImGui::Text("-");
    }

    ImGui::NextColumn();
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
