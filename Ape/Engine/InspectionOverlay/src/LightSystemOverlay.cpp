#include <Ape/Engine/InspectionOverlay/LightSystemOverlay.hpp>

#include <Ape/Engine/InspectionOverlay/ImGuiWindow.hpp>

#include <Ape/World/Scene/LightSystem.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

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
