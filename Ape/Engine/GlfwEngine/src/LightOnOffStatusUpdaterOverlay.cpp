#include "LightOnOffStatusUpdaterOverlay.hpp"

#include <Ape/World/Scene/Light.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

auto LightOnOffStatusUpdaterOverlay::updateLightOnOffStatus(Light & light)
    -> void
{
    auto isTurnedOn = light.isTurnedOn();

    auto formattedStatus = isTurnedOn ? "Turn off" : "Turn on";

    auto const width = ImGui::GetContentRegionAvailWidth();

    if (ImGui::Button(formattedStatus, {width, 0.f}))
    {
        light.toggle();
    }
}

} // namespace ape
