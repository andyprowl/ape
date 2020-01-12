#include <Ape/Engine/InspectionOverlay/LightSystemOverlay.hpp>

#include <Ape/Engine/InspectionOverlay/ImGuiWindow.hpp>

#include <Ape/World/Scene/LightSystem.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

extern float fogDensity;

extern float fogHeightLimit;

extern glm::vec3 fogColor;

LightSystemOverlay::LightSystemOverlay(
    basix::Position<int> const & initialPosition,
    basix::Size<int> const & initialSize,
    LightSystem & lightSystem)
    : initialPosition{initialPosition}
    , initialSize{initialSize}
    , lightSystem{&lightSystem}
    , spotLightPane{lightSystem.spot}
    , pointLightPane{lightSystem.point}
    , directionalLightPane{lightSystem.directional}
{
}

auto LightSystemOverlay::update()
    -> void
{
    auto const window = makeWindow();

    ImGui::BeginTabBar("Light system");

    ImGui::SliderFloat("Fog density", &ape::fogDensity, 0.0f, 0.5f);

    auto currentColor = reinterpret_cast<float *>(&ape::fogColor);

    auto const editorFlags =
        ImGuiColorEditFlags_DisplayRGB |
        ImGuiColorEditFlags_InputRGB |
        ImGuiColorEditFlags_Float |
        ImGuiColorEditFlags_PickerHueWheel;

    ImGui::ColorEdit3("Fog color", currentColor, editorFlags);

    if (ImGui::BeginTabItem("Spot lights"))
    {
        spotLightPane.update();

        ImGui::EndTabItem();
    }
    
    if (ImGui::BeginTabItem("Point lights"))
    {
        pointLightPane.update();

        ImGui::EndTabItem();
    }
    
    if (ImGui::BeginTabItem("Directional lights"))
    {
        directionalLightPane.update();

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
}

auto LightSystemOverlay::makeWindow() const
    -> ImGuiWindow
{
    return ImGuiWindow{"Light system", initialPosition, initialSize};
}

} // namespace ape
