#pragma once

#include "CameraUniform.hpp"
#include "FrameTimeTracker.hpp"
#include "InputHandler.hpp"
#include "KeyboardPublisher.hpp"
#include "LightingUniform.hpp"
#include "MouseWheelPublisher.hpp"
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

    class UniformSet
    {

    public:

        UniformSet(ShaderProgram const & program)
            : camera{program, "camera"}
            , lighting{program, "lighting"}
        {
        }

    public:

        CameraUniform camera;

        LightingUniform lighting;
    
    };

private:
    
    static auto createScene(GLFWwindow & window, ShaderProgram & shader)
        -> Scene;

    auto captureMouse() const
        -> void;

    auto bindMaterialSamplers() const
        -> void;

    auto wasTerminationRequested() const
        -> bool;

    auto processInput()
        -> void;

    auto render()
        -> void;

    auto isWindowReady() const
        -> bool;

    auto clear()
        -> void;

    auto setupCamera()
        -> void;

    auto setupLights()
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

    MouseWheelPublisher wheelPublisher;

    KeyboardPublisher keyboardPublisher;

    ShaderProgram shader;

    Scene scene;

    InputHandler inputHandler;

    FrameTimeTracker timeTracker;

    UniformSet uniforms;

};
