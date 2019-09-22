#include <Ape/Engine.hpp>

#include <Ape/CameraDrivenPipeline.hpp>
#include <Ape/CameraSpotlightSynchronizer.hpp>
#include <Ape/CameraUniform.hpp>
#include <Ape/FrameRateTracker.hpp>
#include <Ape/FrameTimeTracker.hpp>
#include <Ape/InputHandler.hpp>
#include <Ape/LightingUniform.hpp>
#include <Ape/Scene.hpp>
#include <Ape/SceneRenderer.hpp>
#include <Ape/ScopedSignalConnection.hpp>
#include <Ape/ShaderProgram.hpp>
#include <Ape/Window.hpp>

#include "GLFW.hpp"

namespace
{

auto createShader()
    -> ShaderProgram
{
    auto shader = ShaderProgram{"Object.Vertex.glsl", "Object.Fragment.glsl"};

    shader.use();

    shader.getUniform<int>("material.diffuse") = 0;

    shader.getUniform<int>("material.specular") = 1;

    return shader;
}

} // unnamed namespace

class Engine::Impl
{

public:

    Impl(Window & window, Scene & scene, InputHandler & inputHandler)
        : window{&window}
        , scene{&scene}
        , inputHandler{&inputHandler}
        , shader{createShader()}
        , renderer{shader, {0.0f, 0.0f, 0.0f}}
        , rateTracker{timeTracker, 500}
        , resizeHandlerConnection{registerWindowResizeHandler()}
    {
        window.captureMouse();
    }

    auto run()
        -> void
    {
        setViewport();

        while (!wasTerminationRequested())
        {
            processInput();

            render();

            recordFrameDuration();

            reportFramesPerSecond();
        }
    }
    
    auto registerWindowResizeHandler()
        -> ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (Size<int> const & /*newSize*/)
        {
            setViewport();
        });
    }

    auto setViewport()
        -> void
    {
        auto const size = window->getSize();

        if (size.height == 0)
        {
            return;
        }

        glViewport(0, 0, size.width, size.height);

        auto const aspectRatio = window->getAspectRatio();

        scene->cameraSystem.activeCamera->setAspectRatio(aspectRatio);
    }

    auto wasTerminationRequested() const
        -> bool
    {
        return window->shouldClose();
    }

    auto processInput()
        -> void
    {
        glfwPollEvents();

        auto const lastFrameDuration = timeTracker.getLastFrameDuration();

        inputHandler->processInput(lastFrameDuration);
    }

    auto render()
        -> void
    {
        if (!isWindowReady())
        {
            return;
        }

        renderer.render(*scene);

        window->swapBuffers();
    }

    auto isWindowReady() const
        -> bool
    {
        auto const size = window->getSize();

        return (size.height > 0);
    }

    auto recordFrameDuration()
        -> void
    {
        timeTracker.update();
    }

    auto reportFramesPerSecond()
        -> void
    {
        rateTracker.update();
    }

private:
    
    Window * window;

    Scene * scene;

    InputHandler * inputHandler;

    ShaderProgram shader;

    SceneRenderer renderer;

    FrameTimeTracker timeTracker;

    FrameRateTracker rateTracker;

    ScopedSignalConnection resizeHandlerConnection;

};

Engine::Engine(Window & window, Scene & scene, InputHandler & inputHandler)
    : impl{std::make_unique<Impl>(window, scene, inputHandler)}
{
}

Engine::Engine(Engine &&) noexcept = default;

auto Engine::operator = (Engine &&) noexcept
    -> Engine & = default;

Engine::~Engine() = default;

auto Engine::run()
    -> void
{
    return impl->run();
}