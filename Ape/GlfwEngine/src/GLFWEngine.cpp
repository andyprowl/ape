#include <Ape/GlfwEngine/GLFWEngine.hpp>

#include <Ape/Rendering/SceneRenderer.hpp>
#include <Ape/Scene/Scene.hpp>
#include <Ape/UpdateHandling/InputHandler.hpp>
#include <Ape/Windowing/Window.hpp>

#include <Foundational/Signal/ScopedSignalConnection.hpp>
#include <Foundational/Time/FrequencyTracker.hpp>
#include <Foundational/Time/Stopwatch.hpp>
#include <Foundational/Time/TimeIntervalTracker.hpp>

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
        -> ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (Size<int> const & size)
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

    auto setViewport(Size<int> const & size)
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

    Stopwatch stopwatch;
    
    Window * window;

    SceneRenderer * renderer;

    InputHandler * inputHandler;

    TimeIntervalTracker timeTracker;

    FrequencyTracker rateTracker;

    ScopedSignalConnection resizeHandlerConnection;

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
