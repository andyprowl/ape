#pragma once

#include <Ape/Keyboard.hpp>
#include <Ape/Position.hpp>
#include <Ape/Signal.hpp>
#include <Ape/Size.hpp>

#include <stdexcept>
#include <string>

struct GLFWwindow;

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

    Signal<auto (Size<int> const & newSize) -> void> onResize;

    Signal<auto (double offset) -> void> onMouseWheel;

    Signal<auto (Key key, KeyAction action, KeyModifier modifier) -> void> onKeyboard;

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