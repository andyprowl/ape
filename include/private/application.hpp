#pragma once

#include "camera.hpp"
#include "input.hpp"
#include "shader.hpp"
#include "frame.hpp"
#include "world.hpp"

class Application
{

public:

    class ShaderSuite
    {

    public:

        ShaderSuite(ShaderProgram objectShader, ShaderProgram lampShader)
            : objectShader{std::move(objectShader)}
            , lampShader{std::move(lampShader)}
        {
        }

    public:

        ShaderProgram objectShader;

        ShaderProgram lampShader;

    };

public:

    Application();

    Application(Application &&) = delete;

    auto operator = (Application &&) 
        -> Application & = delete;

    ~Application();

    auto run()
        -> void;

private:

    static auto createShaderProgramSuite()
        -> ShaderSuite;

    static auto createObjectShaderProgram()
        -> ShaderProgram;

    static auto createLampShaderProgram()
        -> ShaderProgram;

    static auto createWorld(ShaderSuite const & shaders)
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

    ShaderSuite shaders;

    World world;

    Camera camera;

    InputHandler inputHandler;

    FrameTimeTracker timeTracker;

};
