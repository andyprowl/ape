#include <Ape/Engine/InspectionOverlay/LightDirectionUpdaterOverlay.hpp>

#include <Ape/World/Scene/DirectionalLight.hpp>
#include <Ape/World/Scene/SpotLight.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

template<typename LightType>
auto LightDirectionUpdaterOverlay<LightType>::updateLightDirection(LightType & light)
    -> void
{
    auto position = light.getDirection();

    if (ImGui::SliderFloat3("Direction", reinterpret_cast<float *>(&position), -1.0f, +1.0f))
    {
        light.setDirection(position);
    }
}

template class LightDirectionUpdaterOverlay<DirectionalLight>;

template class LightDirectionUpdaterOverlay<SpotLight>;

} // namespace ape
