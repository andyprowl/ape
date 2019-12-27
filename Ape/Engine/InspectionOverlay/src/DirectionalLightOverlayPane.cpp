#include <Ape/Engine/InspectionOverlay/DirectionalLightOverlayPane.hpp>

#include <DearImGui/imgui.h>

#include <cassert>

namespace ape
{

auto DirectionalLightOverlayPane::updateSelectedLightProperties()
    -> void
{
    auto const light = getSelectedLight();

    assert(light != nullptr);

    onOffStatusUpdater.updateLightOnOffStatus(*light);

    shadowCastingStatusUpdater.updateLightShadowCastingStatus(*light);

    colorUpdater.updateLightColor(*light);

    updateViewProperties(*light);
}

auto DirectionalLightOverlayPane::updateViewProperties(DirectionalLight & light)
    -> void
{
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    if (!ImGui::TreeNode("View"))
    {
        return;
    }

    directionUpdater.updateLightDirection(light);

    ImGui::TreePop();
}

} // namespace ape
