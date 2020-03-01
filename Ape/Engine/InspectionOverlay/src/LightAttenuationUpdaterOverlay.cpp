#include <Ape/Engine/InspectionOverlay/LightAttenuationUpdaterOverlay.hpp>

#include <Ape/World/Scene/PointLight.hpp>
#include <Ape/World/Scene/SpotLight.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

template<typename LightType>
auto LightAttenuationUpdaterOverlay<LightType>::updateLightAttenuation(LightType & light)
    -> void
{
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    if (not ImGui::TreeNode("Attenuation"))
    {
        return;
    }

    auto attenuation = light.getAttenuation();

    auto const isConstantComponentUpdated = updateConstantComponent(attenuation);

    auto const isQuadraticComponentUpdated = updateQuadraticComponent(attenuation);

    if (isConstantComponentUpdated || isQuadraticComponentUpdated)
    {
        light.setAttenuation(attenuation);
    }

    ImGui::TreePop();
}

template<typename LightType>
auto LightAttenuationUpdaterOverlay<LightType>::updateConstantComponent(Attenuation & attenuation)
    -> bool
{
    auto component = reinterpret_cast<float *>(&attenuation.constant);

    return ImGui::SliderFloat("Constant", component, 0.0f, 1.0f);
}

template<typename LightType>
auto LightAttenuationUpdaterOverlay<LightType>::updateQuadraticComponent(Attenuation & attenuation)
    -> bool
{
    auto component = reinterpret_cast<float *>(&attenuation.quadratic);

    return ImGui::SliderFloat("Quadratic", component, 0.0f, 1.0f);
}

template class LightAttenuationUpdaterOverlay<PointLight>;

template class LightAttenuationUpdaterOverlay<SpotLight>;

} // namespace ape
