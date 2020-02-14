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
class Scene;
class Window;

class Engine
{

public:

    Engine(
        Window & window,
        EventSystem & eventSystem,
        std::unique_ptr<Renderer> renderer,
        std::unique_ptr<InputHandler> inputHandler,
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

    auto getRenderer() const
        -> Renderer &;

    auto getInputHandler()
        -> InputHandler &;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

auto getScene(Engine const & engine)
    -> Scene &;

auto getWindow(Engine const & engine)
    -> Window &;

} // namespace ape
