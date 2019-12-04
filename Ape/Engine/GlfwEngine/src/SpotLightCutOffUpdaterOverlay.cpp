#include "SpotLightCutoffUpdaterOverlay.hpp"

#include <DearImGui/imgui.h>

#include <glm/trigonometric.hpp>

namespace ape
{

auto SpotLightCutoffUpdaterOverlay::updateLightCutoff(SpotLight & light)
    -> void
{
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    if (!ImGui::TreeNode("Cutoff angle"))
    {
        return;
    }

    auto cutoff = light.getCutoff();

    auto const isOuterAngleUpdated = updateOuterAngle(cutoff);

    auto const isInnerAngleUpdated = updateInnerAngle(cutoff);

    if (isOuterAngleUpdated || isInnerAngleUpdated)
    {
        light.setCutoff(cutoff);
    }

    ImGui::TreePop();
}

auto SpotLightCutoffUpdaterOverlay::updateOuterAngle(SpotLight::CutoffAngle & cutoff)
    -> bool
{
    auto component = reinterpret_cast<float *>(&cutoff.outer);

    return ImGui::SliderFloat("Outer angle", component, cutoff.inner, glm::radians(90.0f));
}

auto SpotLightCutoffUpdaterOverlay::updateInnerAngle(SpotLight::CutoffAngle & cutoff)
    -> bool
{
    auto component = reinterpret_cast<float *>(&cutoff.inner);

    return ImGui::SliderFloat("Inner angle", component, glm::radians(0.0f), cutoff.outer);
}

} // namespace ape
