#pragma once

#include <memory>

class InputHandler;
class Scene;
class Window;

class Engine
{

public:

    Engine(Window & window, Scene & scene, InputHandler & inputHandler);

    Engine(Engine const &) = delete;

    Engine(Engine &&) noexcept;

    auto operator = (Engine const &) 
        -> Engine & = delete;

    auto operator = (Engine &&) noexcept
        -> Engine &;

    ~Engine();

    auto run()
        -> void;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};
