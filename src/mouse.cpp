#include "Mouse.hpp"

#include "Window.hpp"

MouseTracker::MouseTracker(GLFWwindow & window)
    : window{&window}
    , lastPosition{getCurrentMousePosition(window)}
    , lastMovement{0.0, 0.0}
{
}

auto MouseTracker::update()
    -> void
{
    auto const currentPosition = getCurrentMousePosition(*window);

    lastMovement = Movement{currentPosition.x - lastPosition.x, currentPosition.y - lastPosition.y};

    lastPosition = currentPosition;
}

auto MouseTracker::getLastMovement() const
    -> Movement
{
    return lastMovement;
}

auto getCurrentMousePosition(GLFWwindow & window)
    -> Position
{
    auto x = double{};

    auto y = double{};

    glfwGetCursorPos(&window, &x, &y);

    return {x, y};
}