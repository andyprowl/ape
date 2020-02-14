#include <Ape/Engine/InspectionOverlay/SpotLightOverlayPane.hpp>

#include <DearImGui/imgui.h>

#include <cassert>

namespace ape
{

auto SpotLightOverlayPane::updateSelectedLightProperties()
    -> void
{
    auto const light = getSelectedLight();

    assert(light != nullptr);

    onOffStatusUpdater.updateLightOnOffStatus(*light);

    shadowCastingStatusUpdater.updateLightShadowCastingStatus(*light);

    colorUpdater.updateLightColor(*light);

    attenuationUpdater.updateLightAttenuation(*light);

    cutoffUpdater.updateLightCutoff(*light);

    updateViewProperties(*light);
}

auto SpotLightOverlayPane::updateViewProperties(SpotLight & light)
    -> void
{
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    if (not ImGui::TreeNode("View"))
    {
        return;
    }

    positionUpdater.updateLightPosition(light);

    directionUpdater.updateLightDirection(light);

    ImGui::TreePop();
}

} // namespace ape
