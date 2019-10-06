#include <GlfwEngine/GLFWEngine.hpp>

#include <InputHandling/InputHandler.hpp>
#include <InputHandling/Window.hpp>

#include <Rendering/SceneRenderer.hpp>

#include <Scene/Scene.hpp>

#include <Signal/ScopedSignalConnection.hpp>

#include <Time/FrequencyTracker.hpp>
#include <Time/Stopwatch.hpp>
#include <Time/TimeIntervalTracker.hpp>

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

        glViewport(0, 0, size.width, size.height);

        auto const aspectRatio = window->getAspectRatio();

        auto & scene = getScene(*renderer);

        for (auto & camera : scene.getCameras())
        {
            camera.setAspectRatio(aspectRatio);
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