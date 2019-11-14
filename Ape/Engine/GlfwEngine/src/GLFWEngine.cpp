#include <Ape/Engine/GlfwEngine/GLFWEngine.hpp>

#include <Ape/Engine/UpdateHandling/InputHandler.hpp>

#include <Ape/Rendering/Rendering/SceneRenderer.hpp>
#include <Ape/Rendering/Windowing/Window.hpp>
#include <Ape/World/Scene/Scene.hpp>

#include <Basix/Signal/ScopedSignalConnection.hpp>
#include <Basix/Time/FrequencyTracker.hpp>
#include <Basix/Time/Stopwatch.hpp>
#include <Basix/Time/TimeIntervalTracker.hpp>

#include "GLFW.hpp"

namespace ape
{

class GLFWEngine::Impl
{

public:

    Impl(Window & window, SceneRenderer & renderer, InputHandler & inputHandler)
        : window{&window}
        , renderer{&renderer}
        , inputHandler{&inputHandler}
        , timeTracker{stopwatch}
        , rateTracker{timeTracker, 500}
        , resizeHandlerConnection{registerWindowResizeHandler()}
        , stopBeforeNextIteration{false}
    {
        setViewport();
    }

    auto start()
        -> void
    {
        while (!shouldStop())
        {
            processOneFrame();
        }
    }

    auto stop()
        -> void
    {
        stopBeforeNextIteration = true;
    }

private:
    
    auto registerWindowResizeHandler()
        -> basix::ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (basix::Size<int> const & size)
        {
            setViewport(size);
        });
    }

    auto shouldStop() const
        -> bool
    {
        return (stopBeforeNextIteration || window->isClosing());
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

    auto setViewport(basix::Size<int> const & size)
        -> void
    {
        if (size.height == 0)
        {
            return;
        }

        renderer->setViewport({{0, 0}, size});

        auto const aspectRatio = window->getAspectRatio();

        auto & scene = getScene(*renderer);

        for (auto & camera : scene.getCameras())
        {
            auto const perspective = camera.getProjection().tryAs<PerspectiveProjection>();

            if (perspective != nullptr)
            {
                perspective->setAspectRatio(aspectRatio);
            }
        }
    }

    auto processInput()
        -> void
    {
        glfwPollEvents();

        auto const lastFrameDuration = timeTracker.getLastIntervalDuration();

        inputHandler->onFrame(lastFrameDuration);
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
        timeTracker.tick();
    }

    auto reportFramesPerSecond()
        -> void
    {
        rateTracker.update();
    }

private:

    basix::Stopwatch stopwatch;
    
    Window * window;

    SceneRenderer * renderer;

    InputHandler * inputHandler;

    basix::TimeIntervalTracker timeTracker;

    basix::FrequencyTracker rateTracker;

    basix::ScopedSignalConnection resizeHandlerConnection;

    bool stopBeforeNextIteration;

};

GLFWEngine::GLFWEngine(Window & window, SceneRenderer & renderer, InputHandler & inputHandler)
    : impl{std::make_unique<Impl>(window, renderer, inputHandler)}
{
}

GLFWEngine::GLFWEngine(GLFWEngine &&) noexcept = default;

auto GLFWEngine::operator = (GLFWEngine &&) noexcept
    -> GLFWEngine & = default;

GLFWEngine::~GLFWEngine() = default;

auto GLFWEngine::start()
    -> void
{
    return impl->start();
}

auto GLFWEngine::stop()
    -> void
{
    return impl->stop();
}

} // namespace ape
