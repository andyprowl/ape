#include <Core/Engine.hpp>

#include <Core/CameraDrivenPipeline.hpp>
#include <Core/CameraSpotlightSynchronizer.hpp>
#include <Core/CameraUniform.hpp>
#include <Core/FrameRateTracker.hpp>
#include <Core/InputHandler.hpp>
#include <Core/LightingUniform.hpp>
#include <Core/Scene.hpp>
#include <Core/SceneRenderer.hpp>
#include <Core/ScopedSignalConnection.hpp>
#include <Core/Stopwatch.hpp>
#include <Core/TimeIntervalTracker.hpp>
#include <Core/Window.hpp>

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

private:
    
    auto registerWindowResizeHandler()
        -> ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (Size<int> const & size)
        {
            setViewport(size);
        });
    }

    auto processOneFrame()
        -> void
    {
        processInput();

        render();

        recordFrameDuration();

        reportFramesPerSecond();
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

        auto & scene = getScene(*renderer);

        for (auto & camera : scene.cameras)
        {
            camera.setAspectRatio(aspectRatio);
        }
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

        auto const lastFrameDuration = timeTracker.getLastIntervalDuration();

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

    TimeIntervalTracker timeTracker;

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
