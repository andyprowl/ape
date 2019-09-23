#pragma once

#include <Ape/Keyboard.hpp>
#include <Ape/Window.hpp>

#include <QObject>
#include <QOpenGLWidget>

#include <vector>

namespace ape::qt
{

class WidgetWindow : public QObject, public Window
{

    Q_OBJECT

public:

    WidgetWindow(QOpenGLWidget & widget);

    WidgetWindow(WidgetWindow const & rhs) = delete;

    WidgetWindow(WidgetWindow && rhs) noexcept = default;

    auto operator = (WidgetWindow const & rhs)
        -> WidgetWindow & = delete;

    auto operator = (WidgetWindow && rhs) noexcept
        -> WidgetWindow & = default;

    ~WidgetWindow() = default;

    auto getAspectRatio() const
        -> float override;

    auto getCenter() const
        -> Position<double> override;

    auto getSize() const
        -> Size<int> override;

    auto getPosition() const
        -> Position<int> override;

    auto getMousePosition() const
        -> Position<double> override;

    auto isKeyPressed(Key key) const
        -> bool override;

    auto shouldClose() const
        -> bool override;

    auto requestClosure()
        -> void override;

    auto swapBuffers()
        -> void override;

    auto isMouseCaptured() const
        -> bool override;

    auto captureMouse()
        -> void override;

    auto releaseMouse()
        -> void override;

    auto isFullScreen() const
        -> bool override;

    auto setFullScreen()
        -> void override;

    auto exitFullScreen()
        -> void override;

    auto makeCurrent()
        -> void override;

    auto pollEvents()
        -> void override;

protected:
    
    // virtual (from QObject)
    auto eventFilter(QObject * obj, QEvent * e)
        -> bool override;

private:

    QOpenGLWidget * widget;

    std::vector<ape::KeyStatus> keyStatus;

};

} // namespace ape::qt
