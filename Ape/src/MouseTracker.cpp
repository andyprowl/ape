#include <Ape/MouseTracker.hpp>

#include <Ape/Window.hpp>

namespace ape
{

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

    lastMovement = Offset{currentPosition.x - lastPosition.x, currentPosition.y - lastPosition.y};

    lastPosition = currentPosition;
}

auto MouseTracker::getLastMovement() const
    -> Offset
{
    return lastMovement;
}

} // namespace ape
