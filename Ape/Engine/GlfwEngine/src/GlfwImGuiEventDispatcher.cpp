#include <Ape/Engine/GlfwEngine/GlfwImGuiEventDispatcher.hpp>

#include "GlfwImGuiBinding.hpp"

#include <Ape/Engine/GlfwEngine/GlfwWindow.hpp>

namespace ape
{

GlfwImGuiEventDispatcher::GlfwImGuiEventDispatcher()
    : window{nullptr}
{
}

// virtual (from ImGuiEventDispatcher)
auto GlfwImGuiEventDispatcher::initialize(Window & newWindow)
    -> void
{
    auto & glfwWindow = dynamic_cast<GlfwWindow &>(newWindow);

    window = &glfwWindow;

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow.getGlfwHandle(), true);
}

// virtual (from ImGuiEventDispatcher)
auto GlfwImGuiEventDispatcher::onFrameBegin()
    -> void
{
    ImGui_ImplGlfw_NewFrame();
}

// virtual (from ImGuiEventDispatcher)
auto GlfwImGuiEventDispatcher::onFrameEnd()
    -> void
{
}

} // namespace ape
