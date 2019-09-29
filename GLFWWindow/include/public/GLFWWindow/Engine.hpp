#pragma once

#include <memory>

namespace ape
{

class InputHandler;
class SceneRenderer;
class Window;

class Engine
{

public:

    Engine(Window & window, SceneRenderer & renderer, InputHandler & inputHandler);

    Engine(Engine const &) = delete;

    Engine(Engine &&) noexcept;

    auto operator = (Engine const &) 
        -> Engine & = delete;

    auto operator = (Engine &&) noexcept
        -> Engine &;

    ~Engine();

    auto start()
        -> void;

    auto stop()
        -> void;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape
