#include <Ape/Engine/UpdateHandling/MovementTracker.hpp>

#include <Ape/Engine/Windowing/Window.hpp>

namespace ape
{

MovementTracker::MovementTracker(basix::Position2d<int> initialPosition)
    : lastPosition{initialPosition}
    , lastMovement{0, 0}
{
}

auto MovementTracker::updatePosition(basix::Position2d<int> position)
    -> basix::Offset2d<int>
{
    lastMovement = basix::Offset2d{position.x - lastPosition.x, position.y - lastPosition.y};

    lastPosition = position;

    return lastMovement;
}

auto MovementTracker::getLastMovement() const
    -> basix::Offset2d<int>
{
    return lastMovement;
}

} // namespace ape
