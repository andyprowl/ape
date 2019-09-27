#pragma once

#include <Ape/Offset.hpp>
#include <Ape/Position.hpp>

namespace ape
{

class MovementTracker
{

public:

    explicit MovementTracker(Position<int> initialPosition);

    auto updatePosition(Position<int> position)
        -> Offset<int>;

    auto getLastMovement() const
        -> Offset<int>;

private:

    Position<int> lastPosition;

    Offset<int> lastMovement;

};

} // namespace ape
