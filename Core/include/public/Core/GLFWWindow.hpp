#pragma once

#include <Core/Window.hpp>

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
