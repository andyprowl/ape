#pragma once

#include <Basix/Signal/Signal.hpp>

#include <memory>

namespace ape
{

class EventSystem;
class FrameLoop;
class ImGuiEventDispatcher;
class InputHandler;
class Renderer;
class Window;

class Engine
{

public:

    Engine(
        Window & window,
        EventSystem & eventSystem,
        Renderer & renderer,
        InputHandler & inputHandler,
        ImGuiEventDispatcher & dispatcher,
        FrameLoop & frameLoop);

    Engine(Engine const &) = delete;

    Engine(Engine &&) noexcept;

    auto operator = (Engine const &) 
        -> Engine & = delete;

    auto operator = (Engine &&) noexcept
        -> Engine &;

    ~Engine();

    auto run()
        -> void;

    auto pause()
        -> void;

    auto isInspectionOverlayVisible() const
        -> bool;

    auto showInspectionOverlay()
        -> void;

    auto hideInspectionOverlay()
        -> void;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape
