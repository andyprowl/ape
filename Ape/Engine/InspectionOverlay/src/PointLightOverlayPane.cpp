#include <Ape/Engine/InspectionOverlay/PointLightOverlayPane.hpp>

#include <DearImGui/imgui.h>

#include <cassert>

namespace ape
{

auto PointLightOverlayPane::updateSelectedLightProperties()
    -> void
{
    auto const light = getSelectedLight();

    assert(light != nullptr);

    onOffStatusUpdater.updateLightOnOffStatus(*light);

    shadowCastingStatusUpdater.updateLightShadowCastingStatus(*light);

    colorUpdater.updateLightColor(*light);

    attenuationUpdater.updateLightAttenuation(*light);

    updateViewProperties(*light);
}

auto PointLightOverlayPane::updateViewProperties(PointLight & light)
    -> void
{
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    if (not ImGui::TreeNode("View"))
    {
        return;
    }

    positionUpdater.updateLightPosition(light);

    ImGui::TreePop();
}

} // namespace ape
