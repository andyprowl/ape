#pragma once

#include "Camera.hpp"
#include "FrameTimeTracker.hpp"
#include "InputHandler.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"

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

    static auto createScene(GLFWwindow & window)
        -> Scene;

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

    auto drawScene()
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

    Scene scene;

    InputHandler inputHandler;

    FrameTimeTracker timeTracker;

};
