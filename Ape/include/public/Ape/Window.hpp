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
        -> Size<int> = 0;

    virtual auto getPosition() const
        -> Position<int> = 0;

    virtual auto getMousePosition() const
        -> Position<int> = 0;

    virtual auto isFullScreen() const
        -> bool = 0;

    virtual auto setFullScreen()
        -> void = 0;

    virtual auto exitFullScreen()
        -> void = 0;

    virtual auto isKeyPressed(Key key) const
        -> bool = 0;

};

} // namespace ape
