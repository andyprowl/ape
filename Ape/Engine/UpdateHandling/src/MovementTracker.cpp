#include <Ape/Engine/UpdateHandling/MovementTracker.hpp>

#include <Ape/Engine/Windowing/Window.hpp>

namespace ape
{

MovementTracker::MovementTracker(basix::Position<int> initialPosition)
    : lastPosition{initialPosition}
    , lastMovement{0, 0}
{
}

auto MovementTracker::updatePosition(basix::Position<int> position)
    -> basix::Offset<int>
{
    lastMovement = basix::Offset{position.x - lastPosition.x, position.y - lastPosition.y};

    lastPosition = position;

    return lastMovement;
}

auto MovementTracker::getLastMovement() const
    -> basix::Offset<int>
{
    return lastMovement;
}

} // namespace ape
