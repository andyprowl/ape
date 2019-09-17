#pragma once

#include "Camera.hpp"
#include "FrameTimeTracker.hpp"
#include "InputHandler.hpp"
#include "LightingUniform.h"
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
            : cameraPosition{program, "viewPosition"}
            , cameraTransformation{program, "transform.camera"}
            , lighting{program, "lighting"}
        {
        }

    public:

        Uniform<glm::vec3> cameraPosition;

        Uniform<glm::mat4> cameraTransformation;

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

    ShaderProgram shader;

    Scene scene;

    InputHandler inputHandler;

    FrameTimeTracker timeTracker;

    UniformSet uniforms;

};
