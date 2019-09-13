#pragma once

#include "Camera.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Frame.hpp"
#include "World.hpp"

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

    static auto createWorld(GLFWwindow & window, ShaderProgram const & shader)
        -> World;

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

    auto setupPointLights()
        -> void;

    auto setupSpotLights()
        -> void;

    auto setupDirectionalLights()
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

    InputHandler inputHandler;

    FrameTimeTracker timeTracker;

};
