#include <Ape/Engine/InspectionOverlay/LightShadowCastingStatusUpdaterOverlay.hpp>

#include <Ape/World/Scene/Light.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

auto LightShadowCastingStatusUpdaterOverlay::updateLightShadowCastingStatus(Light & light)
    -> void
{
    auto isCastingShadow = light.isCastingShadow();

    if (ImGui::Checkbox("Casts shadow", &isCastingShadow))
    {
        light.toggleShadowCasting();
    }
}

} // namespace ape
