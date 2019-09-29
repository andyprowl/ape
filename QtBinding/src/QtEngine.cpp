#include <QtBinding/QtEngine.hpp>

#include <Core/CameraSelector.hpp>
#include <Core/InputHandler.hpp>
#include <Core/Scene.hpp>
#include <Core/SceneRenderer.hpp>
#include <Core/Stopwatch.hpp>
#include <Core/TimeIntervalTracker.hpp>
#include <Core/Window.hpp>

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

        glViewport(0, 0, size.width, size.height);

        updateSelectedCameraAspectRatio(size);
    }

    auto start()
        -> void
    {
        loopTimerConnection = connect(&heartbeat, &QTimer::timeout, [this] 
        {
            auto const lastFrameDuration = timeTracker.tick();

            inputHandler->onFrame(lastFrameDuration);

            window->makeCurrent();

            renderer->render();

            window->swapBuffers();
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
        -> ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (Size<int> const & newSize)
        {
            updateSelectedCameraAspectRatio(newSize);
        });
    }

    auto updateSelectedCameraAspectRatio(Size<int> const & size)
        -> void
    {
        auto const camera = renderer->getCameraSelector().getActiveCamera();

        if (camera == nullptr)
        {
            return;
        }

        auto const aspectRatio = static_cast<float>(size.width) / size.height;

        camera->setAspectRatio(aspectRatio);
    }

private:

    Window * window;
    
    SceneRenderer * renderer;
    
    InputHandler * inputHandler;

    Stopwatch stopwatch;

    TimeIntervalTracker timeTracker;

    QTimer heartbeat;

    QMetaObject::Connection loopTimerConnection;

    ScopedSignalConnection resizeHandlerConnection;

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
