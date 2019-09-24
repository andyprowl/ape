#pragma once

#include <Ape/Offset.hpp>
#include <Ape/Position.hpp>

namespace ape
{

class Window;

class MouseTracker
{

public:

    explicit MouseTracker(Window & window);

    auto update()
        -> void;

    auto getLastMovement() const
        -> Offset;

private:

    Window * window;

    Position<double> lastPosition;

    Offset lastMovement;

};

} // namespace ape
