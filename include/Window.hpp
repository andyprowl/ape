#pragma once

#include "Position.hpp"
#include "Signal.hpp"
#include "Size.hpp"

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

class CouldNotInitializeGLAD : public std::logic_error
{

public:

    CouldNotInitializeGLAD()
        : logic_error{"Failed to create initialize the GLAD framework"}
    {
    }

};

class Window
{

public:

    Window(std::string const & title, bool createAsFullScreen);

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

    auto getKeyStatus(int key) const
        -> int;

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

public:

    Signal<auto (Size<int> const & newSize) -> void> onResize;

    Signal<auto (double offset) -> void> onMouseWheel;

    Signal<auto (int key, int scancode, int action, int mods) -> void> onKeyboard;

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
