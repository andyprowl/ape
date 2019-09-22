#include <Ape/MouseTracker.hpp>

#include <Ape/Window.hpp>

MouseTracker::MouseTracker(Window & window)
    : window{&window}
    , lastPosition{window.getMousePosition()}
    , lastMovement{0.0, 0.0}
{
}

auto MouseTracker::update()
    -> void
{
    auto const currentPosition = window->getMousePosition();

    lastMovement = Movement{currentPosition.x - lastPosition.x, currentPosition.y - lastPosition.y};

    lastPosition = currentPosition;
}

auto MouseTracker::getLastMovement() const
    -> Movement
{
    return lastMovement;
}
