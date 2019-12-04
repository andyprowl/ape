#include "LightPositionUpdaterOverlay.hpp"

#include <Ape/World/Scene/PointLight.hpp>
#include <Ape/World/Scene/SpotLight.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

template<typename LightType>
auto LightPositionUpdaterOverlay<LightType>::updateLightPosition(LightType & light)
    -> void
{
    auto position = light.getPosition();

    if (ImGui::SliderFloat3("Position", reinterpret_cast<float *>(&position), -25.0f, +25.0f))
    {
        light.setPosition(position);
    }
}

template class LightPositionUpdaterOverlay<PointLight>;

template class LightPositionUpdaterOverlay<SpotLight>;

} // namespace ape
