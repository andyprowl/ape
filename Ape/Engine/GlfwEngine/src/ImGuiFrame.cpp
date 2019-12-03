#include "ImGuiFrame.hpp"

#include "GlfwImGuiBinding.hpp"
#include "OpenGLImGuiBinding.hpp"

#include <utility>

namespace ape
{

ImGuiFrame::ImGuiFrame()
    : isValid{true}
{
    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();
        
    ImGui::NewFrame();
}

ImGuiFrame::ImGuiFrame(ImGuiFrame && rhs) noexcept
    : isValid{std::exchange(rhs.isValid, false)}
{
}

auto ImGuiFrame::operator = (ImGuiFrame && rhs) noexcept
    -> ImGuiFrame &
{
    isValid = std::exchange(rhs.isValid, false);

    return *this;
}

ImGuiFrame::~ImGuiFrame()
{
    if (isValid)
    {
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

} // namespace ape
