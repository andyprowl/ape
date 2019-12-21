#pragma once

namespace ape
{

class Window;

class ImGuiEventDispatcher
{

public:

    virtual ~ImGuiEventDispatcher() = default;

    virtual auto initialize(Window & window)
        -> void = 0;

    virtual auto onFrameBegin()
        -> void = 0;

    virtual auto onFrameEnd()
        -> void = 0;

};

} // namespace ape
