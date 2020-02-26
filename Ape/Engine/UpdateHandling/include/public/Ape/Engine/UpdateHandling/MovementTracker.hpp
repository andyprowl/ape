#pragma once

#include <Basix/Mathematics/Offset.hpp>
#include <Basix/Mathematics/Position.hpp>

namespace ape
{

class MovementTracker
{

public:

    explicit MovementTracker(basix::Position2d<int> initialPosition);

    auto updatePosition(basix::Position2d<int> position)
        -> basix::Offset2d<int>;

    auto getLastMovement() const
        -> basix::Offset2d<int>;

private:

    basix::Position2d<int> lastPosition;

    basix::Offset2d<int> lastMovement;

};

} // namespace ape
