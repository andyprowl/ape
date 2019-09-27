#include <Ape/MovementTracker.hpp>

#include <Ape/Window.hpp>

namespace ape
{

MovementTracker::MovementTracker(Position<int> initialPosition)
    : lastPosition{initialPosition}
    , lastMovement{0, 0}
{
}

auto MovementTracker::updatePosition(Position<int> position)
    -> Offset<int>
{
    lastMovement = Offset{position.x - lastPosition.x, position.y - lastPosition.y};

    lastPosition = position;

    return lastMovement;
}

auto MovementTracker::getLastMovement() const
    -> Offset<int>
{
    return lastMovement;
}

} // namespace ape
