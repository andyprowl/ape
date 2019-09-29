#pragma once

#include <memory>

namespace ape
{

class InputHandler;
class SceneRenderer;
class Window;

class GLFWEngine
{

public:

    GLFWEngine(Window & window, SceneRenderer & renderer, InputHandler & inputHandler);

    GLFWEngine(GLFWEngine const &) = delete;

    GLFWEngine(GLFWEngine &&) noexcept;

    auto operator = (GLFWEngine const &) 
        -> GLFWEngine & = delete;

    auto operator = (GLFWEngine &&) noexcept
        -> GLFWEngine &;

    ~GLFWEngine();

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
