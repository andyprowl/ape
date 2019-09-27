#pragma once

#include <Ape/RenderingContext.hpp>
#include <Ape/Stopwatch.hpp>
#include <Ape/TimeIntervalTracker.hpp>
#include <Ape/Window.hpp>

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

namespace ape
{

class CameraSelector;
class InputHandler;
class Scene;
class StandardShaderProgram;

} // namespace ape

namespace ape::qt
{

class SceneWidgetNotEngaged : public std::logic_error
{

public:

    SceneWidgetNotEngaged()
        : logic_error{"The widget is not associated to a scene"}
    {
    }

};

class SceneWidget : public QOpenGLWidget, public Window, private QOpenGLFunctions
{

    Q_OBJECT;

public:

    SceneWidget(RenderingContext const & renderingContext, QWidget * parent);

    ~SceneWidget();

    auto engage(
        CameraSelector & cameraSelector,
        InputHandler & inputHandler,
        StandardShaderProgram & shader)
        -> void;

    auto getCameraSelector()
        -> CameraSelector &;

private:

    auto startEventLoop()
        -> void;

    auto processOneFrame()
        -> void;

    // virtual (from QOpenGLWidget)
    auto initializeGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto paintGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto focusInEvent(QFocusEvent * e)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto focusOutEvent(QFocusEvent * e)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto resizeEvent(QResizeEvent * e)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto keyPressEvent(QKeyEvent * e)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto keyReleaseEvent(QKeyEvent * e)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto mouseMoveEvent(QMouseEvent * e)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto wheelEvent(QWheelEvent * e)
        -> void override;

    // virtual (from Window)
    auto getAspectRatio() const
        -> float override;

    // virtual (from Window)
    auto getSize() const
        -> Size<int> override;

    // virtual (from Window)
    auto getPosition() const
        -> Position<int> override;

    // virtual (from Window)
    auto getMousePosition() const
        -> Position<int> override;

    // virtual (from Window)
    auto isFullScreen() const
        -> bool override;

    // virtual (from Window)
    auto setFullScreen()
        -> void override;

    // virtual (from Window)
    auto exitFullScreen()
        -> void override;

    // virtual (from Window)
    auto isKeyPressed(Key key) const
        -> bool override;

    template<typename F>
    auto handleKeyEvent(QKeyEvent & e, KeyStatus status, F inputHandlerNotifier)
        -> void;

    auto setKeyStatus(Key key, KeyStatus status)
        -> void;

private:

    class EngineHolder;

private:

    std::shared_ptr<EngineHolder> holder;

    std::vector<ape::KeyStatus> keyStatus;

    RenderingContext renderingContext;

    Stopwatch stopwatch;

    TimeIntervalTracker timeTracker;

};

} // namespace ape::qt
