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
#include <Ape/Stopwatch.hpp>
#include <Ape/Window.hpp>

#include <glad/glad.h>

namespace ape
{

class Engine::Impl
{

public:

    Impl(Window & window, SceneRenderer & renderer, InputHandler & inputHandler)
        : window{&window}
        , renderer{&renderer}
        , inputHandler{&inputHandler}
        , timeTracker{stopwatch}
        , rateTracker{timeTracker, 500}
        , resizeHandlerConnection{registerWindowResizeHandler()}
    {
        setViewport();
    }

    auto run()
        -> void
    {
        while (!wasTerminationRequested())
        {
            processOneFrame();
        }
    }

    auto processOneFrame()
        -> void
    {
        processInput();

        render();

        recordFrameDuration();

        reportFramesPerSecond();
    }

private:
    
    auto registerWindowResizeHandler()
        -> ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (Size<int> const & size)
        {
            setViewport(size);
        });
    }

    auto setViewport()
        -> void
    {
        auto const size = window->getSize();

        return setViewport(size);
    }

    auto setViewport(Size<int> const & size)
        -> void
    {
        if (size.height == 0)
        {
            return;
        }

        glViewport(0, 0, size.width, size.height);

        auto const aspectRatio = window->getAspectRatio();

        auto const & scene = renderer->getScene();

        scene.cameraSystem.activeCamera->setAspectRatio(aspectRatio);
    }

    auto wasTerminationRequested() const
        -> bool
    {
        return window->shouldClose();
    }

    auto processInput()
        -> void
    {
        window->pollEvents();

        auto const lastFrameDuration = timeTracker.getLastFrameDuration();

        auto const durationInSeconds = lastFrameDuration.count() / 1'000'000'000.0;

        inputHandler->processInput(durationInSeconds);
    }

    auto render()
        -> void
    {
        if (!isWindowReady())
        {
            return;
        }

        renderer->render();

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

    Stopwatch stopwatch;
    
    Window * window;

    SceneRenderer * renderer;

    InputHandler * inputHandler;

    FrameTimeTracker timeTracker;

    FrameRateTracker rateTracker;

    ScopedSignalConnection resizeHandlerConnection;

};

Engine::Engine(Window & window, SceneRenderer & renderer, InputHandler & inputHandler)
    : impl{std::make_unique<Impl>(window, renderer, inputHandler)}
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

auto Engine::processOneFrame()
    -> void
{
    return impl->processOneFrame();
}

} // namespace ape
