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

    auto getWindow() const
        -> Window &;

private:

    Window * window;

    Position<double> lastPosition;

    Offset lastMovement;

};

} // namespace ape
