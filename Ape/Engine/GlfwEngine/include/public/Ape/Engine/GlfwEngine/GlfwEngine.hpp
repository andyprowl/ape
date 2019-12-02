#pragma once

#include <memory>

namespace ape
{

class GlfwWindow;
class InputHandler;
class SceneRenderer;

class GlfwEngine
{

public:

    GlfwEngine(GlfwWindow & window, SceneRenderer & renderer, InputHandler & inputHandler);

    GlfwEngine(GlfwEngine const &) = delete;

    GlfwEngine(GlfwEngine &&) noexcept;

    auto operator = (GlfwEngine const &) 
        -> GlfwEngine & = delete;

    auto operator = (GlfwEngine &&) noexcept
        -> GlfwEngine &;

    ~GlfwEngine();

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
