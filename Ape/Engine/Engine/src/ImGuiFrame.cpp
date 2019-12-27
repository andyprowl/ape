#include <Ape/Engine/Engine/ImGuiFrame.hpp>

#include "OpenGLImGuiBinding.hpp"

#include <Ape/Engine/Engine/ImGuiEventDispatcher.hpp>

#include <utility>

namespace ape
{

ImGuiFrame::ImGuiFrame(ImGuiEventDispatcher & dispatcher)
    : dispatcher{&dispatcher}
{
    ImGui_ImplOpenGL3_NewFrame();

    dispatcher.onFrameBegin();

    ImGui::NewFrame();
}

ImGuiFrame::ImGuiFrame(ImGuiFrame && rhs) noexcept
    : dispatcher{std::exchange(rhs.dispatcher, nullptr)}
{
}

auto ImGuiFrame::operator = (ImGuiFrame && rhs) noexcept
    -> ImGuiFrame &
{
    dispatcher = std::exchange(rhs.dispatcher, nullptr);

    return *this;
}

ImGuiFrame::~ImGuiFrame()
{
    if (dispatcher != nullptr)
    {
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        dispatcher->onFrameEnd();
    }
}

} // namespace ape
