#pragma once

#include <Ape/Engine/Engine/ImGuiEventDispatcher.hpp>

namespace ape::qt
{

class QtWindow;

class QtImGuiEventDispatcher : public ImGuiEventDispatcher
{

public:

    QtImGuiEventDispatcher();

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

    QtWindow * window;

};

} // namespace ape::qt
