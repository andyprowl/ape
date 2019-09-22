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
#include <Ape/Window.hpp>

#include "GLFW.hpp"

namespace ape
{

class Engine::Impl
{

public:

    Impl(Window & window, Scene & scene, SceneRenderer & renderer, InputHandler & inputHandler)
        : window{&window}
        , scene{&scene}
        , renderer{&renderer}
        , inputHandler{&inputHandler}
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

        renderer->render(*scene);

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

    SceneRenderer * renderer;

    InputHandler * inputHandler;

    FrameTimeTracker timeTracker;

    FrameRateTracker rateTracker;

    ScopedSignalConnection resizeHandlerConnection;

};

Engine::Engine(
    Window & window,
    Scene & scene,
    SceneRenderer & renderer,
    InputHandler & inputHandler)
    : impl{std::make_unique<Impl>(window, scene, renderer, inputHandler)}
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

} // namespace ape
