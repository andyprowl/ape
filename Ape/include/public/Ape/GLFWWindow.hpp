#pragma once

#include <Ape/Window.hpp>

struct GLFWwindow;

namespace ape
{

class GLFWWindow : public Window
{

public:

    GLFWWindow(std::string const & title, bool createAsFullScreen);

    GLFWWindow(GLFWWindow const & rhs) = delete;

    GLFWWindow(GLFWWindow && rhs) noexcept;

    auto operator = (GLFWWindow const & rhs)
        -> GLFWWindow & = delete;

    auto operator = (GLFWWindow && rhs) noexcept
        -> GLFWWindow &;

    ~GLFWWindow();

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

    WindowArea lastGLFWWindowedArea;

};

} // namespace ape
