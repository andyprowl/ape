#pragma once

#include "CameraUniform.hpp"
#include "FrameTimeTracker.hpp"
#include "LightingUniform.hpp"
#include "SampleAssetCollection.hpp"
#include "SampleInputHandler.hpp"
#include "SampleScene.hpp"
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

    static auto createAssets()
        -> SampleAssetCollection;
    
    static auto createScene(SampleAssetCollection & assets)
        -> SampleScene;

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

    SampleAssetCollection assets;

    SampleScene scene;

    SceneRenderer renderer;

    SampleInputHandler inputHandler;

    FrameTimeTracker timeTracker;

    ScopedSignalConnection resizeHandlerConnection;

};
