#include "ImGuiWindow.hpp"

#include <DearImGui/imgui.h>

namespace ape
{

ImGuiWindow::ImGuiWindow(std::string_view const title)
    : isValid{true}
{
    ImGui::Begin(title.data());
}

ImGuiWindow::ImGuiWindow(
    std::string_view const title,
    basix::Position<int> const & initialPosition,
    basix::Size<int> const & initialSize)
    : ImGuiWindow{title}
{
    ImGui::SetNextWindowPos(
        {static_cast<float>(initialPosition.x), static_cast<float>(initialPosition.y)},
        ImGuiCond_Once);

    ImGui::SetNextWindowSize(
        {static_cast<float>(initialSize.width), static_cast<float>(initialSize.height)},
        ImGuiCond_Once);
}

ImGuiWindow::ImGuiWindow(ImGuiWindow && rhs) noexcept
    : isValid{std::exchange(rhs.isValid, false)}
{
}

auto ImGuiWindow::operator = (ImGuiWindow && rhs) noexcept
    -> ImGuiWindow &
{
    isValid = std::exchange(rhs.isValid, false);

    return *this;
}

ImGuiWindow::~ImGuiWindow()
{
    if (isValid)
    {
        ImGui::End();
    }
}

} // namespace ape
