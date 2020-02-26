#include <Ape/Engine/InspectionOverlay/FogOverlay.hpp>

#include <Ape/Engine/InspectionOverlay/ImGuiWindow.hpp>

#include <Ape/World/Scene/Fog.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

FogOverlay::FogOverlay(
    basix::Position2d<int> const & initialPosition,
    basix::Size2d<int> const & initialSize,
    Fog & fog)
    : initialPosition{initialPosition}
    , initialSize{initialSize}
    , fog{&fog}
{
}

auto FogOverlay::update()
    -> void
{
    auto const window = makeWindow();

    ImGui::SliderFloat("Fog density", &fog->density, 0.0f, 1.0f);

    auto currentColor = reinterpret_cast<float *>(&fog->color);

    auto const editorFlags =
        ImGuiColorEditFlags_DisplayRGB |
        ImGuiColorEditFlags_InputRGB |
        ImGuiColorEditFlags_Float |
        ImGuiColorEditFlags_PickerHueWheel;

    ImGui::ColorEdit3("Fog color", currentColor, editorFlags);
}

auto FogOverlay::makeWindow() const
    -> ImGuiWindow
{
    return ImGuiWindow{"Fog", initialPosition, initialSize};
}

} // namespace ape
