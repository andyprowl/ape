#pragma once

#include <Ape/Windowing/Keyboard.hpp>

#include <Basix/Mathematics/Offset.hpp>
#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>
#include <Basix/Signal/Signal.hpp>

#include <stdexcept>
#include <string>

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

    Window() = default;

    Window(Window const & rhs) = delete;

    Window(Window && rhs) noexcept = default;

    auto operator = (Window const & rhs)
        -> Window & = delete;

    auto operator = (Window && rhs) noexcept
        -> Window & = default;

    virtual ~Window() = default;

    virtual auto getAspectRatio() const
        -> float = 0;

    virtual auto getSize() const
        -> basix::Size<int> = 0;

    virtual auto getPosition() const
        -> basix::Position<int> = 0;

    virtual auto getMousePosition() const
        -> basix::Position<int> = 0;

    virtual auto isFullScreen() const
        -> bool = 0;

    virtual auto setFullScreen()
        -> void = 0;

    virtual auto exitFullScreen()
        -> void = 0;

    virtual auto isKeyPressed(Key key) const
        -> bool = 0;

    virtual auto isMouseCaptured() const
        -> bool = 0;

    virtual auto captureMouse()
        -> void = 0;

    virtual auto releaseMouse()
        -> void = 0;

    virtual auto swapBuffers()
        -> void = 0;

    virtual auto makeCurrent()
        -> void = 0;

    virtual auto close()
        -> void = 0;

    virtual auto isClosing()
        -> bool = 0;

public:

    basix::Signal<auto (basix::Size<int> const & size) -> void> onResize;

    basix::Signal<auto (basix::Offset<int> const & offset) -> void> onMouseWheel;

    basix::Signal<auto (Key key, KeyAction action, KeyModifier modifier) -> void> onKeyboard;

    basix::Signal<auto () -> void> onClose;

    basix::Signal<auto () -> void> onFocusAcquired;

    basix::Signal<auto () -> void> onFocusLost;

};

} // namespace ape
