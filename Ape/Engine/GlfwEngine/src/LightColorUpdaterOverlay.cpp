#include "LightColorUpdaterOverlay.hpp"

#include <Ape/World/Scene/Light.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

auto LightColorUpdaterOverlay::updateLightColor(Light & light)
    -> void
{
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

    if (!ImGui::TreeNode("Color"))
    {
        return;
    }

    auto color = light.getColor();

    auto const isAmbientColorUpdated = updateColor("Ambient", color.ambient);

    auto const isDiffuseColorUpdated = updateColor("Diffuse", color.diffuse);

    auto const isSpecularColorUpdated = updateColor("Specular", color.specular);

    if (isAmbientColorUpdated || isDiffuseColorUpdated || isSpecularColorUpdated)
    {
        light.setColor(color);
    }

    ImGui::TreePop();
}

auto LightColorUpdaterOverlay::updateColor(std::string_view const label, glm::vec3 & color)
    -> bool
{
    auto currentColor = reinterpret_cast<float *>(&color);

    auto const editorFlags =
        ImGuiColorEditFlags_DisplayRGB |
        ImGuiColorEditFlags_InputRGB |
        ImGuiColorEditFlags_Float |
        ImGuiColorEditFlags_PickerHueWheel;

    return ImGui::ColorEdit3(label.data(), currentColor, editorFlags);
}

} // namespace ape
