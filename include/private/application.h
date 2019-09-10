#pragma once

#include "camera.hpp"
#include "input.hpp"
#include "shader.hpp"
#include "tracker.hpp"
#include "world.hpp"

class Application
{

public:

    Application();

    Application(Application &&) = delete;

    auto operator = (Application &&) 
        -> Application & = delete;

    ~Application();

    auto run()
        -> void;

private:

    static auto createShaderProgram()
        -> ShaderProgram;

    static auto buildWorld(ShaderProgram const & program)
        -> World;

    auto shouldClose() const
        -> bool;

    auto processInput()
        -> void;

    auto render()
        -> void;

    auto clear()
        -> void;

    auto setupCamera()
        -> void;

    auto drawWorld()
        -> void;

    auto swapBuffers()
        -> void;

    auto pollEvents()
        -> void;

    auto updateFrameTime()
        -> void;

private:

    GLFWwindow * window;

    ShaderProgram shader;

    World world;

    Camera camera;

    InputHandler inputHandler;

    FrameTimeTracker timeTracker;

};
