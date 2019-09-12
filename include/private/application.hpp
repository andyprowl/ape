#pragma once

#include "camera.hpp"
#include "input.hpp"
#include "shader.hpp"
#include "frame.hpp"
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

    static auto createWorld(ShaderProgram const & shader)
        -> World;

    static auto createCamera(GLFWwindow & window)
        -> Camera;

    auto captureMouse() const
        -> void;

    auto wasTerminationRequested() const
        -> bool;

    auto processInput()
        -> void;

    auto render()
        -> void;

    auto clear()
        -> void;

    auto setupCamera()
        -> void;

    auto setupLights()
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
