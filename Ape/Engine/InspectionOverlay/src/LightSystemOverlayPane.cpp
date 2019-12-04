#include <Ape/Engine/InspectionOverlay/LightSystemOverlayPane.hpp>

#include <Ape/World/Scene/DirectionalLight.hpp>
#include <Ape/World/Scene/PointLight.hpp>
#include <Ape/World/Scene/SpotLight.hpp>

#include <DearImGui/imgui.h>

#include <array>

namespace ape
{

template<typename LightType>
LightSystemOverlayPane<LightType>::LightSystemOverlayPane(std::vector<LightType> & lights)
    : lights{&lights}
    , selectedLight{nullptr}
{
}

template<typename LightType>
auto LightSystemOverlayPane<LightType>::update()
    -> void
{
    updateTogglingPanel();

    ImGui::SameLine();

    updateDetailPanel();
}

template<typename LightType>
auto LightSystemOverlayPane<LightType>::getSelectedLight() const
    -> LightType *
{
    return selectedLight;
}

template<typename LightType>
auto LightSystemOverlayPane<LightType>::updateTogglingPanel()
    -> void
{
    auto const availableSpace = ImGui::GetContentRegionAvail();
    
    auto const leftPaneSize = ImVec2(availableSpace.x * 0.25f, availableSpace.y);

    ImGui::BeginChild("Toggling panel", leftPaneSize, true, 0);

    ImGui::Text("Toggling panel:");

    for (auto & light : *lights)
    {
        auto isChecked = light.isTurnedOn();

        if (ImGui::Checkbox(light.getName().data(), &isChecked))
        {
            light.toggle();
        }
    }

    ImGui::EndChild();
}

template<typename LightType>
auto LightSystemOverlayPane<LightType>::updateDetailPanel()
    -> void
{
    auto const availableSpace = ImGui::GetContentRegionAvail();

    auto const rightPaneSize = ImVec2(availableSpace.x, availableSpace.y);

    ImGui::BeginChild("Light details", rightPaneSize, true, 0);

    updateLightSelector();

    if (selectedLight != nullptr)
    {
        updateSelectedLightProperties();
    }

    ImGui::EndChild();
}

template<typename LightType>
auto LightSystemOverlayPane<LightType>::updateLightSelector()
    -> void
{
    auto const selectedName = (selectedLight != nullptr) ? selectedLight->getName().data() : "";

    if (ImGui::BeginCombo("Selected light", selectedName))
    {
        for (auto & light : *lights)
        {
            ImGui::PushID(static_cast<void *>(&light));
        
            if (ImGui::Selectable(light.getName().data(), &light == selectedLight))
            {
                selectedLight = &light;
            }
        
            ImGui::PopID();
        }

        ImGui::EndCombo();
    }
}

template class LightSystemOverlayPane<DirectionalLight>;

template class LightSystemOverlayPane<PointLight>;

template class LightSystemOverlayPane<SpotLight>;

} // namespace ape
