#include "FrameProfilingOverlay.hpp"

#include "ImGuiWindow.hpp"

#include <Ape/Rendering/Windowing/Window.hpp>

#include <DearImGui/imgui.h>

#include <algorithm>

namespace ape
{

namespace
{

class FrameProfileView
{

public:

    using FrameProfileBuffer = basix::CircularBuffer<basix::TaskProfile>;

    using HistogramType = FrameProfilingOverlay::HistogramType;

public:

    FrameProfileView(FrameProfileBuffer const & frameProfiles, HistogramType const histogramType)
        : frameProfiles{&frameProfiles}
        , histogramType{histogramType}
    {
    }

    auto getProfile(int const index) const
        -> basix::TaskProfile const &
    {
        if (histogramType == HistogramType::stationary)
        {
            return *(frameProfiles->data() + index);
        }
        else
        {
            return (*frameProfiles)[index];
        }
    }

private:

    FrameProfileBuffer const * frameProfiles;

    HistogramType histogramType;

};

auto frameProfileGetter(void * const data, int const index)
    -> float
{
    auto const & profileView = *(reinterpret_cast<FrameProfileView *>(data));

    auto const & profile = profileView.getProfile(index);

    auto const durationInMicroseconds = profile.getDuration().count();

    return durationInMicroseconds / 1'000.f;
}

} // unnamed namespace

FrameProfilingOverlay::FrameProfilingOverlay(
    basix::Position<int> const & initialPosition,
    basix::Size<int> const & initialSize,
    FrameProfileBuffer const & lastFrameProfiles)
    : initialPosition{initialPosition}
    , initialSize{initialSize}
    , lastFrameProfiles{&lastFrameProfiles}
    , doNotRecordFrameProfiles{false}
    , histogramType{HistogramType::circular}
    , maxNumOfPlottedFrames{lastFrameProfiles.capacity() / 2}
    , frameDurationCapInMs{50}
    , selectedFrameIndex{-1}
    , selectedFrameProfile{nullptr}
{
}

auto FrameProfilingOverlay::update()
    -> void
{
    auto const window = makeWindow();

    updatePauseProfiling();

    updateHistogramType();

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
    return selectedFrameProfile;
}

auto FrameProfilingOverlay::makeWindow() const
    -> ImGuiWindow
{
    auto window = ImGuiWindow{"FrameProfiling", initialPosition, initialSize};

    ImGui::Begin("Frame profiling");

    return window;
}

auto FrameProfilingOverlay::updatePauseProfiling()
    -> void
{
    ImGui::Checkbox("Pause frame profiling", &doNotRecordFrameProfiles);

    if (!doNotRecordFrameProfiles)
    {
        selectedFrameIndex = -1;

        selectedFrameProfile = nullptr;
    }
}

auto FrameProfilingOverlay::updateHistogramType()
    -> void
{
    auto useStationaryHistogram = (histogramType == HistogramType::stationary);

    ImGui::Checkbox("Use stationary histogram", &useStationaryHistogram);

    histogramType = (useStationaryHistogram ? HistogramType::stationary : HistogramType::circular);
}

auto FrameProfilingOverlay::updateMaxNumOfPlottedFrames()
    -> void
{
    ImGui::SliderInt(
        "Maximum number of plotted frames",
        &maxNumOfPlottedFrames,
        0,
        lastFrameProfiles->capacity());
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

    auto profileView = FrameProfileView{*lastFrameProfiles, histogramType};

    auto const numOfFramesToPlot = std::min(maxNumOfPlottedFrames, lastFrameProfiles->size());

    auto const availableWindowSpace = ImGui::GetContentRegionAvail();

    ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, {1.0f, 0.3f, 1.0f, 1.0f});

    ImGui::PlotHistogram(
        "Frame time histogram",
        frameProfileGetter,
        static_cast<void *>(&profileView),
        numOfFramesToPlot,
        lastFrameProfiles->size() - numOfFramesToPlot,
        nullptr,
        0.0f,
        static_cast<float>(frameDurationCapInMs),
        {availableWindowSpace.x, availableWindowSpace.y},
        nullptr,
        &selectedFrameIndex);

    ImGui::PopStyleColor();

    if ((selectedFrameIndex >= 0) && ImGui::IsItemClicked())
    {
        selectedFrameProfile = &(*lastFrameProfiles)[selectedFrameIndex];

        doNotRecordFrameProfiles = true;
    }
}

} // namespace ape
