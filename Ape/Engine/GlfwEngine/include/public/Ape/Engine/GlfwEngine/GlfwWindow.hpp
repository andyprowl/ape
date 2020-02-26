#pragma once

#include <Ape/Engine/Windowing/Window.hpp>

#include <string_view>

struct GLFWwindow;

namespace ape
{

class GlfwWindow : public Window
{

public:

    class CreateAsFullscreen
    {

    public:

        explicit CreateAsFullscreen() = default;

    };

public:

    GlfwWindow(std::string_view title, CreateAsFullscreen);

    GlfwWindow(std::string_view title, basix::Size2d<int> const & size);

    GlfwWindow(GlfwWindow const & rhs) = delete;

    GlfwWindow(GlfwWindow && rhs) noexcept;

    auto operator = (GlfwWindow const & rhs)
        -> GlfwWindow & = delete;

    auto operator = (GlfwWindow && rhs) noexcept
        -> GlfwWindow &;

    ~GlfwWindow();

    auto getGlfwHandle() const
        -> GLFWwindow *;

    // virtual (from Window)
    auto getAspectRatio() const
        -> float override;

    // virtual (from Window)
    auto getSize() const
        -> basix::Size2d<int> override;

    // virtual (from Window)
    auto getPosition() const
        -> basix::Position2d<int> override;

    // virtual (from Window)
    auto getMousePosition() const
        -> basix::Position2d<int> override;

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

        WindowArea(basix::Position2d<int> const & position, basix::Size2d<int> const & size)
            : position{position}
            , size{size}
        {
        }

    public:
    
        basix::Position2d<int> position;

        basix::Size2d<int> size;
    
    };

private:

    GlfwWindow(GLFWwindow & handle, bool isFullScreen);

    auto registerEventHandlers()
        -> void;

private:

    GLFWwindow * handle;

    bool isFullScreenModeOn;

    WindowArea lastGLFWWindowedArea;

};

} // namespace ape
