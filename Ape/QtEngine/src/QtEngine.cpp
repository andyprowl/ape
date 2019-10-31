#include <Ape/QtEngine/QtEngine.hpp>

#include <Ape/Rendering/SceneRenderer.hpp>
#include <Ape/Scene/CameraSelector.hpp>
#include <Ape/Scene/Scene.hpp>
#include <Ape/UpdateHandling/InputHandler.hpp>
#include <Ape/Windowing/Window.hpp>

#include <Foundational/Time/Stopwatch.hpp>
#include <Foundational/Time/TimeIntervalTracker.hpp>

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
            setViewport(newSize);

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

        auto perspective = camera->getProjection().tryAs<PerspectiveProjection>();

        if (perspective == nullptr)
        {
            return;
        }

        auto const aspectRatio = static_cast<float>(size.width) / size.height;

        perspective->setAspectRatio(aspectRatio);
    }

    auto setViewport(Size<int> const & size)
        -> void
    {
        auto const origin = Position{0, 0};

        renderer->setViewport({origin, size});
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
