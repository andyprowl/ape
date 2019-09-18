#pragma once

#include "CameraUniform.hpp"
#include "FrameTimeTracker.hpp"
#include "InputHandler.hpp"
#include "LightingUniform.hpp"
#include "Scene.hpp"
#include "SceneRenderer.hpp"
#include "ScopedSignalConnection.hpp"
#include "ShaderProgram.hpp"
#include "Window.hpp"

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

    static auto createShader()
        -> ShaderProgram;
    
    static auto createScene(Window const & window, ShaderProgram & shader)
        -> Scene;

    auto registerWindowResizeHandler()
        -> ScopedSignalConnection;

    auto setViewport()
        -> void;

    auto wasTerminationRequested() const
        -> bool;

    auto processInput()
        -> void;

    auto render()
        -> void;

    auto isWindowReady() const
        -> bool;

    auto recordFrameDuration()
        -> void;

private:

    Window window;

    ShaderProgram shader;

    Scene scene;

    SceneRenderer renderer;

    InputHandler inputHandler;

    FrameTimeTracker timeTracker;

    ScopedSignalConnection resizeHandlerConnection;

};
