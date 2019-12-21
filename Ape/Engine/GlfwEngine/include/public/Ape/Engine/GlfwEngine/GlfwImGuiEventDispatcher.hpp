#pragma once

#include <Ape/Engine/Engine/ImGuiEventDispatcher.hpp>

namespace ape
{

class GlfwWindow;

class GlfwImGuiEventDispatcher : public ImGuiEventDispatcher
{

public:

    GlfwImGuiEventDispatcher();

    // virtual (from ImGuiEventDispatcher)
    auto initialize(Window & newWindow)
        -> void override;

    // virtual (from ImGuiEventDispatcher)
    auto onFrameBegin()
        -> void override;

    // virtual (from ImGuiEventDispatcher)
    auto onFrameEnd()
        -> void override;

private:

    GlfwWindow * window;

};

} // namespace ape
