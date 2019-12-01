#include <Ape/Engine/QtEngine/QtEngine.hpp>

#include <Ape/Engine/UpdateHandling/InputHandler.hpp>

#include <Ape/Rendering/Rendering/SceneRenderer.hpp>
#include <Ape/Rendering/Windowing/Window.hpp>
#include <Ape/World/Scene/CameraSelector.hpp>
#include <Ape/World/Scene/Scene.hpp>

#include <Basix/Time/Stopwatch.hpp>
#include <Basix/Time/TimeIntervalTracker.hpp>

#include <glad/glad.h>

#include <QTimer>

namespace ape::qt
{

class QtEngine::Impl : public QObject
{

    Q_OBJECT

public:

    Impl(Window & window, SceneRenderer & renderer, InputHandler & inputHandler)
        : window{&window}
        , renderer{&renderer}
        , inputHandler{&inputHandler}
        , timeTracker{stopwatch}
        , resizeHandlerConnection{registerWindowResizeHandler()}
    {
        auto const size = window.getSize();

        setViewport(size);
    }

    auto start()
        -> void
    {
        loopTimerConnection = connect(&heartbeat, &QTimer::timeout, [this] 
        {
            processOneFrame();
        });
    
        // The timer's callback will be invoked whenever the application is idle.
        heartbeat.start();
    }

    auto stop()
        -> void
    {
        heartbeat.stop();
    }

private:

    auto registerWindowResizeHandler()
        -> basix::ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (basix::Size<int> const & newSize)
        {
            setViewport(newSize);
        });
    }

    auto setViewport(basix::Size<int> const & size)
        -> void
    {
        auto const origin = basix::Position{0, 0};

        renderer->setViewport({origin, size});

        updateSelectedCameraAspectRatio(size);
    }

    auto updateSelectedCameraAspectRatio(basix::Size<int> const & size)
        -> void
    {
        auto const camera = renderer->getCameraSelector().getActiveCamera();

        if (camera == nullptr)
        {
            return;
        }

        auto perspective = camera->getProjection().tryAs<PerspectiveProjection>();

        if (perspective == nullptr)
        {
            return;
        }

        auto const aspectRatio = static_cast<float>(size.width) / size.height;

        perspective->setAspectRatio(aspectRatio);
    }
    
    auto processOneFrame()
        -> void
    {
        processInput();
        
        render();
    }
        
    auto processInput()
        -> void
    {
        auto const lastFrameDuration = timeTracker.tick();

        inputHandler->onFrame(lastFrameDuration);
    }

    auto render()
        -> void
    {
        window->makeCurrent();

        renderer->render();

        window->swapBuffers();
    }

private:

    Window * window;
    
    SceneRenderer * renderer;
    
    InputHandler * inputHandler;

    basix::Stopwatch stopwatch;

    basix::TimeIntervalTracker timeTracker;

    QTimer heartbeat;

    QMetaObject::Connection loopTimerConnection;

    basix::ScopedSignalConnection resizeHandlerConnection;

};

QtEngine::QtEngine(Window & window, SceneRenderer & renderer, InputHandler & inputHandler)
    : impl{std::make_unique<Impl>(window, renderer, inputHandler)}
{
}

QtEngine::QtEngine(QtEngine &&) noexcept = default;

auto QtEngine::operator = (QtEngine &&) noexcept
    -> QtEngine & = default;

QtEngine::~QtEngine() = default;

auto QtEngine::start()
    -> void
{
    return impl->start();
}

auto QtEngine::stop()
    -> void
{
    return impl->stop();
}

} // namespace ape::qt

#include "QtEngine.moc"
