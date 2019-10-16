#pragma once

#include <Windowing/Window.hpp>

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

namespace ape::qt
{

class QtWindowNotEngaged : public std::logic_error
{

public:

    QtWindowNotEngaged()
        : logic_error{"The widget is not associated to a scene"}
    {
    }

};

class QtWindow : public QOpenGLWidget, public Window, private QOpenGLFunctions
{

    Q_OBJECT;

public:

    explicit QtWindow(QWidget * parent = nullptr);

    ~QtWindow();

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
    auto isKeyPressed(Key key) const
        -> bool override;

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
    auto isMouseCaptured() const
        -> bool override;

    // virtual (from Window)
    auto captureMouse()
        -> void override;

    // virtual (from Window)
    auto releaseMouse()
        -> void override;

    // virtual (from Window)
    auto swapBuffers()
        -> void override;

    // virtual (from Window)
    auto makeCurrent()
        -> void override;

    // virtual (from Window)
    auto close()
        -> void override;

    // virtual (from Window)
    auto isClosing()
        -> bool override;

protected:

    // virtual (from QOpenGLWidget)
    auto initializeGL()
        -> void override;

    // virtual (from QOpenGLWidget)
    auto paintEvent(QPaintEvent *)
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
    auto wheelEvent(QWheelEvent * e)
        -> void override;

    // virtual (from QOpenGLWidget)
    auto closeEvent(QCloseEvent * e)
        -> void override;

private:

    template<typename F>
    auto handleKeyEvent(QKeyEvent & e, KeyStatus status, F inputHandlerNotifier)
        -> void;

    auto setKeyStatus(Key key, KeyStatus status)
        -> void;

private:

    std::vector<ape::KeyStatus> keyStatus;

    bool isMouseGrabbed;

    bool isWindowClosing;

    bool isOpenGLInitialized;

};

} // namespace ape::qt
