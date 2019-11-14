#pragma once

#include <Ape/Windowing/Window.hpp>

#include <string_view>

struct GLFWwindow;

namespace ape
{

class GLFWWindow : public Window
{

public:

    class CreateAsFullscreen
    {

    public:

        explicit CreateAsFullscreen() = default;

    };

public:

    GLFWWindow(std::string_view title, CreateAsFullscreen);

    GLFWWindow(std::string_view title, basix::Size<int> const & size);

    GLFWWindow(GLFWWindow const & rhs) = delete;

    GLFWWindow(GLFWWindow && rhs) noexcept;

    auto operator = (GLFWWindow const & rhs)
        -> GLFWWindow & = delete;

    auto operator = (GLFWWindow && rhs) noexcept
        -> GLFWWindow &;

    ~GLFWWindow();

    // virtual (from Window)
    auto getAspectRatio() const
        -> float override;

    // virtual (from Window)
    auto getSize() const
        -> basix::Size<int> override;

    // virtual (from Window)
    auto getPosition() const
        -> basix::Position<int> override;

    // virtual (from Window)
    auto getMousePosition() const
        -> basix::Position<int> override;

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

private:

    class WindowArea
    {

    public:

        WindowArea(basix::Position<int> const & position, basix::Size<int> const & size)
            : position{position}
            , size{size}
        {
        }

    public:
    
        basix::Position<int> position;

        basix::Size<int> size;
    
    };

private:

    GLFWWindow(GLFWwindow & handle, bool isFullScreen);

    auto registerEventHandlers()
        -> void;

private:

    GLFWwindow * handle;

    bool isFullScreenModeOn;

    WindowArea lastGLFWWindowedArea;

};

} // namespace ape
