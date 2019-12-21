#include <Ape/Engine/QtEngine/QtImGuiEventDispatcher.hpp>

#include "Win32ImGuiBinding.hpp"

#include <Ape/Engine/QtEngine/QtWindow.hpp>

namespace ape::qt
{

QtImGuiEventDispatcher::QtImGuiEventDispatcher()
    : window{nullptr}
{
}

// virtual (from ImGuiEventDispatcher)
auto QtImGuiEventDispatcher::initialize(Window & newWindow)
    -> void
{
    auto & qtWindow = dynamic_cast<QtWindow &>(newWindow);

    window = &qtWindow;

    ImGui_ImplWin32_Init(reinterpret_cast<void *>(window->winId()));
}

// virtual (from ImGuiEventDispatcher)
auto QtImGuiEventDispatcher::onFrameBegin()
    -> void
{
    ImGui_ImplWin32_NewFrame(reinterpret_cast<void *>(window->winId()));
}

// virtual (from ImGuiEventDispatcher)
auto QtImGuiEventDispatcher::onFrameEnd()
    -> void
{
}

} // namespace ape::qt
