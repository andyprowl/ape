#pragma once

#include <Basix/Mathematics/Offset.hpp>
#include <Basix/Mathematics/Position.hpp>

namespace ape
{

class MovementTracker
{

public:

    explicit MovementTracker(basix::Position<int> initialPosition);

    auto updatePosition(basix::Position<int> position)
        -> basix::Offset<int>;

    auto getLastMovement() const
        -> basix::Offset<int>;

private:

    basix::Position<int> lastPosition;

    basix::Offset<int> lastMovement;

};

} // namespace ape
