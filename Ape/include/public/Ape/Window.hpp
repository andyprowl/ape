#pragma once

#include <Ape/Keyboard.hpp>
#include <Ape/Position.hpp>
#include <Ape/Signal.hpp>
#include <Ape/Size.hpp>

#include <functional>
#include <stdexcept>
#include <string>

struct GLFWwindow;

namespace ape
{

class CouldNotCreateWindow : public std::logic_error
{

public:

    CouldNotCreateWindow()
        : logic_error{"Failed to create the OpenGL window"}
    {
    }

};

class Window
{

public:

    Window(std::string const & title, bool createAsFullScreen);

    Window(Window const & rhs) = delete;

    Window(Window && rhs) noexcept;

    auto operator = (Window const & rhs)
        -> Window & = delete;

    auto operator = (Window && rhs) noexcept
        -> Window &;

    ~Window();

    auto getAspectRatio() const
        -> float;

    auto getCenter() const
        -> Position<double>;

    auto getSize() const
        -> Size<int>;

    auto getPosition() const
        -> Position<int>;

    auto getMousePosition() const
        -> Position<double>;

    auto isKeyPressed(Key key) const
        -> bool;

    auto shouldClose() const
        -> bool;

    auto requestClosure()
        -> void;

    auto swapBuffers()
        -> void;

    auto isMouseCaptured() const
        -> bool;

    auto captureMouse()
        -> void;

    auto releaseMouse()
        -> void;

    auto isFullScreen() const
        -> bool;

    auto setFullScreen()
        -> void;

    auto exitFullScreen()
        -> void;

    auto makeCurrent()
        -> void;

public:

    mutable Signal<auto (Size<int> const & newSize) -> void> onResize;

    mutable Signal<auto (double offset) -> void> onMouseWheel;

    mutable Signal<auto (Key key, KeyAction action, KeyModifier modifier) -> void> onKeyboard;

private:

    class WindowArea
    {

    public:

        WindowArea(Position<int> const & position, Size<int> const & size)
            : position{position}
            , size{size}
        {
        }

    public:
    
        Position<int> position;

        Size<int> size;
    
    };

private:

    auto registerEventHandlers()
        -> void;

private:

    GLFWwindow * handle;

    bool isFullScreenModeOn;

    WindowArea lastWindowedArea;

};

auto onKeyPressed(
    Window const & window,
    std::function<auto (Key key, KeyModifier modifier) -> void> handler)
    -> ScopedSignalConnection;

auto onKeyPressed(
    Window const & window,
    Key key,
    std::function<auto (KeyModifier modifier) -> void> handler)
    -> ScopedSignalConnection;

} // namespace ape
